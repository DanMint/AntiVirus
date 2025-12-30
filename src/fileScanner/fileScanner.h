#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <openssl/evp.h>

class FileScanner{
    public:
        FileScanner(const std::string &filePath);

        std::string hashingFucntion(const EVP_MD* algo);

        bool checkMD5Hashes(const std::string &fileMD5Hash);

        bool checkSHA1Hashes(const std::string &fileSHA1Hash);

        bool checkSHA256Hashes(const std::string &fileSHA256Hash);

        void start();

    private:
        const std::string filePath;
};