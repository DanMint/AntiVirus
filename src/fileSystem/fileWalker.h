#pragma once

#include <string>
#include <filesystem>

#include "../utils/runParameters.h"

namespace fs = std::filesystem;

class FileWalker {
    public:
        FileWalker(const RunParameters &currentParameters);

        void start();

    private:
        const RunParameters currentParameters;
};