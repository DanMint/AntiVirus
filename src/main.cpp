#include <iostream>
#include <filesystem>

#include "fileSystem/fileWalker.h"
#include "utils/utils.h"
#include "utils/runParameters.h"

// folder test path: "/Users/daniel/Desktop/Antivirus"

int main(int argc, char *argv[]) {

    RunParameters RunParameters;

    Utils::checkValidityOfArguments(argc, argv, RunParameters);

    FileWalker startWalk(argv[1]);

    return 0;
}