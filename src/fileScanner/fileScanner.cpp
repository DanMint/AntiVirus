#include "fileScanner.h"
#include "../utils/utils.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <filesystem>

FileScanner::FileScanner(const std::string &filePath) : filePath(filePath) {
    #ifdef DEBUG
        std::cout << "Here is the passed: " << this->filePath << '\n';
    #endif
    FileScanner::start();
}

std::string FileScanner::hashingFucntion(const EVP_MD* algo) {
    // Reading the file 8KB at a time (common page-alinged size)
    constexpr size_t BUF_SIZE = 8192;
    unsigned char buffer[BUF_SIZE];

    // Opening the file safely (we know the file exists bc it was passed through fileWalker). This opens the file in Binary mode. Avoids new line translation
    std::ifstream file(this->filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + this->filePath);
    }

    // EVP_MD_CTX is like a hash engine state machine. It stores partial hash state, internal buffers, and algo specific metadata. 
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }

    // Here the specified hash algoriothm (MD5, SHA1 or SHA256) is initiated. This basically tells OpenSSL use the passed algorithm, reset internal state and prepare to accept data
    if (EVP_DigestInit_ex(ctx, algo, nullptr) != 1) {
        // If faliure happens clean up immediately
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    // Looping until EOF or failure
    while (file.good()) {
        // Reading up to 8192 bytes into buffer
        file.read(reinterpret_cast<char*>(buffer), BUF_SIZE);
        // How many bytes are actually read (std::streamsize is of type long on macos)
        std::streamsize bytes = file.gcount();
        // Here is where the hashing starts
        if (bytes > 0) {
            // The hash state gets stores in ctx. It gets updated here. 
            // buffer = the file data itself
            // bytes = how many bytes read
            EVP_DigestUpdate(ctx, buffer, bytes);
        }
    }

    // Here the hash is finalized. digest = "The final fixed-length output of a cryptographic hash function." 
    // Digest lengths for different hash algorithms:
    // MD5 Digest = 16bytes
    // SHA1 Digest = 20bytes
    // SHA256 Digest = 32bytes
    // SHA512 Digest = 64bytes
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    // Copying data out of the CTX state. Also padding, message length encoding, final comporession rounds and final output bytes are written. EVP_DigestFinal_ex finalizes the hash computaion
    if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    // Freeing hash context
    EVP_MD_CTX_free(ctx);

    // Digest is in bytes this translates it to the hash format
    std::ostringstream hex;
    for (unsigned int i = 0; i < digest_len; ++i) {
        hex << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(digest[i]);
    }

    return hex.str();
}

bool FileScanner::checkMD5Hashes(const std::string &fileMD5Hash) {
    // this is the relative path from where the program is executed
    std::filesystem::path folder = "src/fileScanner/Malware-Hash-Database/MD5"; 
    if (!Utils::checkFileExistence(folder)) {
        #ifdef DEBUG
            std::cout << "In checkMD5Hashes function. Passed folder not found. Check where you are running the program from\n";
        #endif

        std::cerr << "Folder not found\n";
        return false;
    }

    #ifdef DEBUG
        std::cout << "In checkMD5Hashes function. Found folder\n";
        std::cout << "Printing out all of the files in folder\n";
        for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
            std::cout << dirEntry.path() << '\n';
        }
    #endif
    
    for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
        if (dirEntry.is_regular_file() && dirEntry.path().extension() == ".txt") {
            std::ifstream file(dirEntry.path());
            if (!file) {
                std::cerr << "Failed to open file\n";
                continue;
            }
            std::string MaliciousMD5Hash;
            while (std::getline(file, MaliciousMD5Hash)) {
               if (MaliciousMD5Hash == fileMD5Hash) {
                    std::cout << "current file is Malicious\n";
                    return true;
               }
            }
        }
        else {
            #ifdef DEBUG
                std::cout << "In checkMD5Hashes function. MD5 folder has a file that is not a .txt file";
            #endif
        }
    }
    
    return false;
}

bool FileScanner::checkSHA1Hashes(const std::string &fileSHA1Hash) {
    std::filesystem::path folder = "src/fileScanner/Malware-Hash-Database/SHA1"; 
    if (!Utils::checkFileExistence(folder)) {
        #ifdef DEBUG
            std::cout << "In checkSHA1Hashes function. Passed folder not found. Check where you are running the program from\n";
        #endif

        std::cerr << "Folder not found\n";
        return false;
    }

    #ifdef DEBUG
        std::cout << "In checkSHA1Hashes function. Found folder\n";
        std::cout << "Printing out all of the files in folder\n";
        for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
            std::cout << dirEntry.path() << '\n';
        }
    #endif
    
    for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
        if (dirEntry.is_regular_file() && dirEntry.path().extension() == ".txt") {
            std::ifstream file(dirEntry.path());
            if (!file) {
                std::cerr << "Failed to open file\n";
                continue;
            }
            std::string MaliciousSHA1Hash;
            while (std::getline(file, MaliciousSHA1Hash)) {
               if (MaliciousSHA1Hash == fileSHA1Hash) {
                    std::cout << "current file is Malicious\n";
                    return true;
               }
            }
        }
        else {
            #ifdef DEBUG
                std::cout << "In checkSHA1Hashes function. MD5 folder has a file that is not a .txt file";
            #endif
        }
    }
    
    return false;
}

bool FileScanner::checkSHA256Hashes(const std::string &fileSHA256Hash) {
    std::filesystem::path folder = "src/fileScanner/Malware-Hash-Database/SHA256"; 
    if (!Utils::checkFileExistence(folder)) {
        #ifdef DEBUG
            std::cout << "In checkSHA512Hashes function. Passed folder not found. Check where you are running the program from\n";
        #endif

        std::cerr << "Folder not found\n";
        return false;
    }

    #ifdef DEBUG
        std::cout << "In checkSHA512Hashes function. Found folder\n";
        std::cout << "Printing out all of the files in folder\n";
        for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
            std::cout << dirEntry.path() << '\n';
        }
    #endif
    
    for (auto const& dirEntry : std::filesystem::directory_iterator{folder}) {
        if (dirEntry.is_regular_file() && dirEntry.path().extension() == ".txt") {
            std::ifstream file(dirEntry.path());
            if (!file) {
                std::cerr << "Failed to open file\n";
                continue;
            }
            std::string MaliciousSHA256Hash;
            while (std::getline(file, MaliciousSHA256Hash)) {
               if (MaliciousSHA256Hash == fileSHA256Hash) {
                    std::cout << "current file is Malicious\n";
                    return true;
               }
            }
        }
        else {
            #ifdef DEBUG
                std::cout << "In checkSHA512Hashes function. MD5 folder has a file that is not a .txt file";
            #endif
        }
    }
    
    return false;
}

void FileScanner::start() {
    const std::string fileMD5Hash = FileScanner::hashingFucntion(EVP_md5());
    const std::string fileSHA1Hash = FileScanner::hashingFucntion(EVP_sha1());
    const std::string fileSHA256Hash = FileScanner::hashingFucntion(EVP_sha256());

     #ifdef DEBUG
        std::cout << "------------------------------------------------\n";
        std::cout << "DEBUGGIN STEP:\n";
        std::cout << "MD5 hash: " << fileMD5Hash << '\n';
        std::cout << "SHA1 hash: " << fileSHA1Hash << '\n';
        std::cout << "SHA256 hash: " << fileSHA256Hash << '\n';
        std::cout << "------------------------------------------------\n";
    #endif

    if (FileScanner::checkMD5Hashes(fileMD5Hash)) {
        std::cout << "Malicious file found as an MD5 hash!\n";
    }
    else if(FileScanner::checkSHA1Hashes(fileSHA1Hash)) {
        std::cout << "Malicious file found as an SHA1 hash!\n";
    }
    else if(FileScanner::checkSHA256Hashes(fileSHA256Hash)) {
        std::cout << "Malicious file found as an SHA256 hash!\n";
    }

}
