#include <iostream>

#include "fileWalker.h"
#include "../utils/utils.h"
#include "../fileScanner/fileScanner.h"

FileWalker::FileWalker(const RunParameters &currentParameters) : currentParameters(currentParameters) {}

void FileWalker::start() {
    std::cout << "Starting to walk files\n";
    if (this->currentParameters.isDirectory) {
        for (auto const &dir_entry : fs::recursive_directory_iterator(this->currentParameters.passedFile)) {
            std::string filePath = dir_entry.path().string();

            if (currentParameters.operation == Operation::Scan) {
                #ifdef DEBUG
                    std::cout << "Starting the scan of " << filePath << '\n';
                #endif
                FileScanner scanFile(filePath);
            }
        }
    }
    else {
        if (currentParameters.operation == Operation::Scan) {
            #ifdef DEBUG
                std::cout << "Starting the scan of " << this->currentParameters.passedFile << '\n';
            #endif
            FileScanner scanFile(this->currentParameters.passedFile);
        }
    }
}
