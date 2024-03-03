#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once

#include <string>

namespace algo {
void encode(FILE* in, FILE* out, FILE* table);
void decode(FILE* in, FILE* out, FILE* table);
}  // namespace algo
