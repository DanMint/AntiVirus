#include <iostream>

#include "fileWalker.h"

FileWalker::FileWalker(const std::string &startingDir) : startingDir(startingDir) {
    if (FileWalker::checkFileExistence(this->startingDir)) {
        FileWalker::start();
    }
    else {
        std::cerr << "Given file/directory does not exist\n";
    }
}

bool FileWalker::checkFileExistence(const fs::path& p, fs::file_status s) {
    std::cout << "Checking file: " << p;
    if (fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
        std::cout << "File exists\n";
        return true;
    }
    else {
        std::cout << "File does not exist\n";
       return false;
    }
        
}

void FileWalker::start() {
    std::cout << "Starting to walk files\n";
}