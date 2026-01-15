#pragma once

enum class Operation {
    Empty = -1,
    Scan = 0,
    Hash = 1,
    Encrypt = 2
};

enum class FileType {
    RegularFile = 0,
    Directory = 1
};