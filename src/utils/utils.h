#pragma once 

#include <filesystem>

#include "runParameters.h"

namespace Utils {

    namespace fs = std::filesystem;

    bool checkValidityOfArguments(const int argc, char* const argv[], RunParameters &currentRunParamters);

    bool checkFileExistence(const fs::path& p, fs::file_status s = fs::file_status{});

    void fileType(const fs::file_status& s, RunParameters &currentRunParamters);

    void printRunParameters(RunParameters &currentRunParameters);
}
