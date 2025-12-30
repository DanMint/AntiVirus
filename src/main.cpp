#include <iostream>
#include <filesystem>

#include "fileSystem/fileWalker.h"
#include "utils/utils.h"
#include "utils/runParameters.h"
// for testing remove when merging PR
#include "fileScanner/fileScanner.h"

// folder test path: "/Users/daniel/Desktop/Antivirus"

int main(int argc, char *argv[]) {

    RunParameters RunParameters;

    Utils::checkValidityOfArguments(argc, argv, RunParameters);

    FileScanner fileScanner(RunParameters.passedFile);
    
    // FileWalker startWalk(RunParameters);

    return 0;
}