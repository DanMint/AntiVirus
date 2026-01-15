#include <iostream>
#include <filesystem>

#include "utils.h"
#include "operation.h"

// basic parse of the passed arguments by user. The passed arguments look as follows:
// ./antivirus "<file/folder location>" <operation type>
bool Utils::checkValidityOfArguments(const int argc, char* const argv[], RunParameters &currentRunParamters) {
    if (argc <= 2) {
        std::cerr << "Not enough argumnets\n";
        return false;
    }
    else if (argc > 3) {
        std::cerr << "Too many arguments\n";
        return false;
    }
    else {
        // first checking if file/directory exists
        const std::string inputFile = argv[1];

        if (!Utils::checkFileExistence(inputFile)) {
            std::cerr << "File does not exist...\n";
            return false;
        }

        currentRunParamters.passedFile = inputFile;
        Utils::fileType(fs::status(inputFile), currentRunParamters);

        #ifdef DEBUG
            Utils::printRunParameters(currentRunParamters);
        #endif
        
        // checking which operation is chosen creating if/else for debug purposes
        const int operationChosen = std::stoi(argv[2]);

        switch(operationChosen) {
            case -1:
                #ifdef DEBUG
                    std::cout << "Operaion chosen by user is EMPTY(-1)\n";
                #endif
                return false;
            
            case 0:
                #ifdef DEBUG
                    std::cout << "Operaion chosen by user is SCAN(0)\n";
                #endif
                currentRunParamters.operation = Operation::Scan;
                break;

            case 1:
                 #ifdef DEBUG
                    std::cout << "Operaion chosen by user is HASH(1)\n";
                #endif
                currentRunParamters.operation = Operation::Hash;
                break;

            case 2:
                #ifdef DEBUG
                    std::cout << "Operaion chosen by user is ENCRYPT(2)\n";
                #endif
                currentRunParamters.operation = Operation::Encrypt;
                break;
            
            default:
                #ifdef DEBUG
                    std::cout << "Operaion chosen by user is  UNDEFINED(" << operationChosen << ")\n";
                #endif
                return false;
        }
        
        // all checks passed 
        return true;
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