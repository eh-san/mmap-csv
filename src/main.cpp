#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <filesystem>

#define CSV_INPUT_COLUMN_SIZE 2

// Function to split a string based on a delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to process CSV file and update duration information
void processCSV(const std::string& csvFileName, std::unordered_map<int, std::vector<int>>& durationMap, const std::vector<int> &vIDs) {
    // Open the CSV file using mmap
    int fd = open(csvFileName.c_str(), O_RDONLY);
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
    


    while (std::getline(csvStream, line)) {
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() == CSV_INPUT_COLUMN_SIZE) {
            int id = std::stoi(tokens[0]);
            if (std::find(vIDs.begin(), vIDs.end(), id) != vIDs.end())
            {
                int duration = std::stoi(tokens[1]);
                durationMap[id].push_back(duration);
            }
        }
    }

    // Unmap the file
    munmap(fileData, fileSize);
}

std::vector<std::string> findCSVFiles(const std::string& path) {
    std::vector<std::string> csvFiles;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.find("input-") == 0 && filename.size() > 4 &&
                    filename.compare(filename.size() - 4, 4, ".csv") == 0) {
                    csvFiles.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << std::endl;
    }

    return csvFiles;
}

int main() {
    // Read the list of IDs
    std::vector<int> idList;
    std::ifstream idFile("example/ids.csv");
    std::string idLine;
    
    // drop the is.csv header
    std::getline(idFile, idLine);
    std::cout << "header: " << idLine << std::endl;

    while (std::getline(idFile, idLine)) {
        idList.push_back(std::stoi(idLine));
    }

    std::cout << idList.size() << std::endl;

    std::vector<std::string> csvFiles = findCSVFiles("example/");

    if (csvFiles.empty())
    {
        std::cout << "No CSV input files found in the specified path.\n";
        return EXIT_FAILURE;
    }

    // Create a map to store durations for each ID
    std::unordered_map<int, std::vector<int>> durationMap;

    // Process each input file
    for (const auto &csvFileName : csvFiles) {
        processCSV(csvFileName, durationMap, idList);
    }

    // Create the output CSV file
    std::ofstream outputFile("output.csv");
    outputFile << "id,last-durations,longest-durations\n";

    // Write results to the output file
    for (const auto& entry : durationMap) {
        const auto& id = entry.first;
        const auto& durations = entry.second;

        // Write id
        outputFile << id << ",";

        // Write last durations
        for (size_t i = 0; i < durations.size(); ++i) {
            outputFile << durations[i];
            if (i < durations.size() - 1) {
                outputFile << "|";
            }
        }
        outputFile << ",";

        // Write longest durations
        auto longestDurations = durations;
        std::sort(longestDurations.rbegin(), longestDurations.rend());
        for (size_t i = 0; i < longestDurations.size(); ++i) {
            outputFile << longestDurations[i];
            if (i < longestDurations.size() - 1) {
                outputFile << "|";
            }
        }

        // End the line
        outputFile << "\n";
    }

    // Close the output file
    outputFile.close();

    std::cout << "Output written to output.csv\n";

    return 0;
}
