#include "file.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <filesystem>


std::optional<size_t> File::countLines(const std::string_view filePath)
{
    std::ifstream file(filePath.data());
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return std::nullopt;
    }

    size_t lineCount = 0;
    std::string line;

    while (std::getline(file, line))
        ++lineCount;

    file.close();
    return lineCount;
}

std::vector<std::string> File::findLists(const std::string_view path, const std::string_view name, const std::string_view extension)
{
    std::vector<std::string> listFiles;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(path.data())) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (filename.find(name) == 0 && filename.size() > 4 &&
                    filename.compare(filename.size() - 4, 4, extension) == 0) {
                    listFiles.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error accessing directory: " << ex.what() << std::endl;
    }

    return listFiles;
}