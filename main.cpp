#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "src/algo.h"
template <typename T>
struct Flag {
    std::string shortcut;
    std::string descryption;
    T action;
};
void flag_parse(int argc, const char* argv[]) {
    const Flag<std::string (*)(const char*, _IO_FILE*)> one_arg_flags[] = {
        {"-e", "encode data", algo::encode},
        {"-d", "decode data", algo::decode},
    };

    std::map<std::string, std::string (*)(const char*, _IO_FILE*)>
        flags_with_arg_map;

    for (auto [shortcut, desc, action] : one_arg_flags) {
        flags_with_arg_map[shortcut] = action;
    }

    FILE* out = stdout;
    auto set_out = [&out](const char* path) { out = fopen(path, "w+"); };
    const Flag<std::function<void(const char*)>> io_args_flags[] = {
        {"-o", "output file", set_out},
    };

    std::map<std::string, std::function<void(const char*)>> io_args_flags_map;

    for (auto [shortcut, desc, action] : io_args_flags) {
        io_args_flags_map[shortcut] = action;
    }

    std::stack<std::pair<std::string (*)(const char*, _IO_FILE*), std::string>>
        call_stack;
    for (int i = 1; i < argc - 1; i++) {
        const char* s = argv[i];
        if (flags_with_arg_map.contains(s)) {
            const char* param = argv[++i];
            call_stack.push({flags_with_arg_map[s], param});
        } else if (io_args_flags_map.contains(s)) {
            const char* path = argv[++i];
            io_args_flags_map[s](path);
        }
    }
    while (!call_stack.empty()) {
        auto p = call_stack.top();
        std::cout << p.first(p.second.c_str(), out);
        call_stack.pop();
    }
}
int main(int argc, const char* argv[]) {
    flag_parse(argc, argv);
}