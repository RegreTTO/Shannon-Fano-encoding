#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once
#include <fstream>
namespace file{
    const char* read_file(FILE* desc);
}