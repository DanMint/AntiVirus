#include "fileScanner.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>


FileScanner::FileScanner(const std::string &filePath) : filePath(filePath) {
    // For debugging
    std::cout << "Here is the passed: " << this->filePath << '\n';
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

void FileScanner::start() {
    std::string md5 = FileScanner::hashingFucntion(EVP_md5());
    std::string sha1 = FileScanner::hashingFucntion(EVP_sha1());
    std::string sha256 = FileScanner::hashingFucntion(EVP_sha256());

    std::cout << "------------------------------------------------\n";
    std::cout << "DEBUGGIN STEP:\n";
    std::cout << "MD5 hash: " << md5 << '\n';
    std::cout << "MD5 hash: " << sha1 << '\n';
    std::cout << "MD5 hash: " << sha256 << '\n';
    std::cout << "------------------------------------------------\n";

}
