#include <iostream>
#include <filesystem>

#include "utils.h"

void Utils::checkValidityOfArguments(const int argc, char* const argv[], RunParameters &currentRunParamters) {
    if (argc == 1) {
        std::cerr << "Not enough argumnets\n";
        return;
    }
    else if (argc > 2) {
        std::cerr << "Too many arguments\n";
        return;
    }
    else {
        // first checking if file/directory even exists
        const std::string inputFile = argv[1];
        currentRunParamters.passedFile = inputFile;

        if (!Utils::checkFileExistence(inputFile)) {
            std::cerr << "File does not exist...\n";
            return;
        }
        Utils::fileType(fs::status(inputFile), currentRunParamters);
        Utils::printRunParameters(currentRunParamters);
    }
}

bool Utils::checkFileExistence(const fs::path& p, fs::file_status s) {
    std::cout << "Checking file: " << p;
    if (fs::status_known(s) ? fs::exists(s) : fs::exists(p)) {
        #ifdef DEBUG
            std::cout << "File exists\n";
        #endif
        return true;
    }
    else {
        std::cout << "File does not exist\n";
       return false;
    }
}

void Utils::fileType(const fs::file_status& s, RunParameters &currentRunParamters) {
    if (fs::is_regular_file(s)) {
        currentRunParamters.isFile = true;
    }
    else if(fs::is_directory(s)) {
        currentRunParamters.isDirectory = true;
    }
}

void Utils::printRunParameters(RunParameters &currentRunParameters) {
    std::cout << "------------------------------\n";
    std::cout << "Printing current parametrs...\n";
    std::cout << "Passed file: " << currentRunParameters.passedFile << '\n';
    std::cout << "File type: " << ((currentRunParameters.isFile == true) ? "File" : "Directory") << '\n';
    std::cout << "File empty: " << currentRunParameters.isEmpty << '\n';
}