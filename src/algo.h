#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once

#include <string>

namespace algo {
std::string encode(const char* msg);
std::string decode(const char*  code);
}  // namespace algo
