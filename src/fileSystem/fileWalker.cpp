#include <iostream>

#include "fileWalker.h"
#include "../utils/utils.h"

FileWalker::FileWalker(const RunParameters &currentParameters) : currentParameters(currentParameters) {}

void FileWalker::start() {
    std::cout << "Starting to walk files\n";
    if (this->currentParameters.isDirectory) {
        for (auto const &dir_entry : fs::recursive_directory_iterator(this->currentParameters.passedFile)) {
            std::cout << dir_entry << "\n";
        }
    }
}
