#include <iostream>
#include "csv.hpp"

int main() {
    // Read the list of IDs
    CSV csv("example/ids.csv");

    std::vector<std::string> inputFiles = csv.findLists("example/", "input-", ".csv");

    if (inputFiles.empty()) {
        std::cout << "No CSV input files found in the specified path.\n";
        exit(EXIT_FAILURE);
    }

    // Process each input file
    for (const auto& inputFileName : inputFiles)
        csv.process(inputFileName);

    // Write to output file
    csv.write();

    return 0;
}
