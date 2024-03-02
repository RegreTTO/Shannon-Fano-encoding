#include <iostream>
#include <string>
#include <unordered_map>
#include "src/algo.h"
void flag_parse(int argc, const char* argv[]) {
    std::unordered_map<std::string, std::string (*)(const char *)>
        flags_with_arg = {
            {"-e", algo::encode},
            {"-d", algo::decode}
    };

    for (int i = 1; i < argc - 1; i++) {
        const char* s = argv[i];
        if (flags_with_arg.contains(s)) {
            const char* param = argv[++i];
            std::cout << flags_with_arg[s](param) << ' ';
        }
    }
}
int main(int argc, const char* argv[]) {
    flag_parse(argc, argv);
}