#pragma once

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileWalker {
    public:
        FileWalker(const std::string &startingDir);

        bool checkFileExistence(const fs::path& p, fs::file_status s = fs::file_status{});

        void start();

    private:
        const std::string startingDir;
};