#!/bin/bash

clang++ -std=c++20 src/main.cpp src/fileScanner/fileScanner.cpp src/fileSystem/fileWalker.cpp src/utils/utils.cpp -I src -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lcrypto -o antivirus
./antivirus "/Users/daniel/Desktop/Antivirus/src/main.cpp"