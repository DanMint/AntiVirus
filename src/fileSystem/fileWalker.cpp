#include <iostream>

#include "fileWalker.h"
#include "../utils/utils.h"

FileWalker::FileWalker(const std::string &startingDir) : startingDir(startingDir) {
    if (Utils::checkFileExistence(this->startingDir)) {
        FileWalker::start();
    }
    else {
        std::cerr << "Given file/directory does not exist\n";
    }
}

void FileWalker::start() {
    std::cout << "Starting to walk files\n";
}


