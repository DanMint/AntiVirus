#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <openssl/evp.h>

class FileScanner{
    public:
        FileScanner(const std::string &filePath);

        std::string hashingFucntion(const EVP_MD* algo);

        void start();

    private:
        const std::string filePath;
};