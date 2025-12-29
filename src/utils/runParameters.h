#pragma once

#include <string>

struct RunParameters {
    std::string passedFile;
    bool isFile;
    bool isDirectory;
    bool isEmpty;

    RunParameters() : passedFile(""), isFile(false), isDirectory(false), isEmpty(true) {}
};
