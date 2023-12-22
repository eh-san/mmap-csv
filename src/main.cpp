#include <iostream>
#include "csv.hpp"

int main() {
    // Read the list of IDs
    CSV csv("example/ids.csv");

    std::vector<std::string> csvFiles = csv.findLists("example/", "input-", ".csv");

    if (csvFiles.empty()) {
        std::cout << "No CSV input files found in the specified path.\n";
        exit(EXIT_FAILURE);
    }

    // Process each input file
    for (const auto& csvFileName : csvFiles)
        csv.process(csvFileName);

    csv.writeOutput();

    return 0;
}
