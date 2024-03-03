#include "file.h"
#include <fstream>
#include <iostream>
namespace file {

const char* read_file(FILE* desc) {
    char* buf;
    // Get file size
    fseek(desc, 0, SEEK_END);
    long file_size = ftell(desc);
    rewind(desc);

    buf = new char[file_size];
    if (buf == NULL) {
        std::cerr << "Memory allocation error";
        fclose(desc);
        exit(2);
        return nullptr;
    }
    if (fread(buf, 1, file_size, desc) != file_size) {
        std::cerr << "Input file is corrupted!\n";
        fclose(desc);
        free(buf);
        exit(2);
        return nullptr;
    }
    return buf;
}
}  // namespace file