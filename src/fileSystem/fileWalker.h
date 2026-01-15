#pragma once

#include <string>
#include <filesystem>

#include "../utils/runParameters.h"

namespace fs = std::filesystem;

class FileWalker {
    public:
        FileWalker(const RunParameters &currentParameters);

        void fileScanner(const std::string &fileLocation);

        void start();

    private:
        const RunParameters currentParameters;
};