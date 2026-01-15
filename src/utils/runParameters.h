#pragma once

#include <string>

#include "operation.h"

struct RunParameters {
    std::string passedFile;
    bool isFile;
    bool isDirectory;
    bool isEmpty;
    Operation operation;

    RunParameters() : passedFile(""), isFile(false), isDirectory(false), isEmpty(true), operation(Operation::Empty) {}
};
