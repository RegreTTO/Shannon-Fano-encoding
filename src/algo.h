#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once

#include <string>

namespace algo {
std::string encode(const char* msg, FILE* out);
std::string decode(const char* code, FILE* out);
}  // namespace algo
