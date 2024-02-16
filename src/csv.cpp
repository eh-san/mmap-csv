#include "csv.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <fstream>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

constexpr int CSV_INPUT_COLUMN_SIZE = 2;

CSV::CSV(const std::string_view idFilePath)
{
    // Read the list of IDs
    std::optional<size_t> idCount = countLines(idFilePath);

    if (!idCount.has_value()) {
        std::cerr << "The file in path: " << idFilePath << " is empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<int> idList(idCount.value(), std::numeric_limits<int>::max());

    std::ifstream idFile(idFilePath.data());
    std::string idLine;

    // drop the ids.csv header
    std::getline(idFile, idLine);
    std::cout << "header: " << idLine << std::endl;

    for (auto& id : idList) {
        std::getline(idFile, idLine);
        id = std::atoi(idLine.data());
    }

    std::cout << idList.size() << std::endl;



    // Map to store durations for each ID
    m_durationMap.reserve(idCount.value());

    for (const auto& id : idList)
        m_durationMap[id].emplace_back(0);

}

std::vector<std::string> CSV::split(const std::string_view s, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s.data());
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(std::move(token));
    }
    return tokens;
}

void CSV::process(const std::string_view csvFileName)
{
    // Open the CSV file using mmap
    int fd = open(csvFileName.data(), O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    off_t fileSize = lseek(fd, 0, SEEK_END);
    char* fileData = static_cast<char*>(mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0));

    close(fd);

    // Parse the CSV data
    std::istringstream csvStream(fileData);
    std::string line;

    // drop the header of the input csv file
    std::getline(csvStream, line);
    std::cout << "header: " << line << std::endl;

    auto it = m_durationMap.end();
    while (std::getline(csvStream, line)) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() == CSV_INPUT_COLUMN_SIZE) {
            int id = std::stoi(tokens[0]);
            it = m_durationMap.find(id);
            if (it != m_durationMap.end())
                it->second.emplace_back(std::stoi(tokens[1]));
        }
    }

    // Unmap the file
    munmap(fileData, fileSize);
}

void CSV::write() const
{
    // Create the output CSV file
    std::ofstream outputFile("output.csv");
    outputFile << "id,last-durations,longest-durations\n";

    // Write results to the output file
    for (auto [id, durations] : m_durationMap) {

        // Write id
        outputFile << id << ",";

        // Write last durations
        std::string last_duration;
        for (const auto& duration : durations)
            last_duration.append(std::to_string(duration)).append("|");
        
        // replace last '|' with ','
        last_duration.erase(last_duration.size() - 1).append(",");
        outputFile << last_duration;

        // Write longest durations
        std::string longest_duration;
        std::sort(durations.rbegin(), durations.rend());
        for (const auto& duration : durations)
            longest_duration.append(std::to_string(duration)).append("|");

        // remove last '|'
        longest_duration.erase(longest_duration.size() - 1);
        outputFile << longest_duration;

        // End the line
        outputFile << "\n";
    }

    // Close the output file
    outputFile.close();

    std::cout << "Output written to output.csv\n";
}