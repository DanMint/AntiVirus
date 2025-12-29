#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileWalker {
    public:
        FileWalker(const std::string &startingDir);

        void start();

    private:
        const std::string startingDir;
};