#include <any>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include "src/algo.h"

#define PRINT_HELP                      \
    {                                   \
        printf("THIS IS HELP MENU!\n"); \
        print_help(std::vector{help});  \
        print_help(one_arg_flags);      \
        print_help(io_args_flags);      \
    }

template <typename T>
struct Flag {
    std::string shortcut;
    std::string descryption;
    T action;
};
template <typename T>
void print_help(std::vector<Flag<T>> flags) {
    std::cout << setiosflags(std::ios::left);
    for (Flag<T> flag : flags) {
        std::cout << std::setw(15) << flag.shortcut.c_str()
                  << flag.descryption.c_str() << std::endl;
    }
}
int flag_parse(int argc, const char* argv[]) {
    const Flag<void (*)()> help = {"-h, --help", "prints help menu"};
    std::vector<Flag<void (*)(FILE*, FILE*, FILE*)>> one_arg_flags = {
        {"-e", "encode data. Enter a raw data if '-i' flag is not set!",
         algo::encode},
        {"-d", "decode data. Enter a raw data if '-i' flag is not set!",
         algo::decode},
    };

    std::map<std::string, void (*)(FILE*, FILE*, FILE*)> flags_with_arg_map;
    for (auto [shortcut, desc, action] : one_arg_flags) {
        flags_with_arg_map[shortcut] = action;
    }

    const char* input_filepath = "stdin";
    const char* output_filepath = "stdout";
    const char* table_filepath = "stdout";
    FILE* out = stdout;
    FILE* in = stdin;
    FILE* table = stdout;

    auto set_out = [&out, &output_filepath](const char* path) {
        out = fopen(path, "w+");
        if (out == NULL) {
            std::cerr << "[!] Out file is not exist!\n";
            exit(2);
        }
        output_filepath = path;
    };
    auto set_in = [&in, &input_filepath](const char* path) {
        in = fopen(path, "r");
        if (in == NULL) {
            std::cerr << "[!] Input file is not exist!\n";
            exit(2);
        }
        input_filepath = path;
    };
    auto set_table = [&table, &table_filepath](const char* path) {
        table = fopen(path, "r+");
        if (table == NULL) {
            FILE* tmp = fopen(path, "w+");
            fclose(tmp);
            table = fopen(path, "r+");
        }
        table_filepath = path;
    };
    std::vector<Flag<std::function<void(const char*)>>> io_args_flags = {
        {"-o", "output file", set_out  },
        {"-i", "input file",  set_in   },
        {"-t", "table file",  set_table}
    };

    std::map<std::string, std::function<void(const char*)>> io_args_flags_map;

    for (auto [shortcut, desc, action] : io_args_flags) {
        io_args_flags_map[shortcut] = action;
    }

    std::stack<std::pair<void (*)(FILE*, FILE*, FILE*), FILE*>> call_stack;

#define invalidArg                                               \
    {                                                            \
        printf("[!] Expected and argument after %s\n", argv[i]); \
        exit(1);                                                 \
    }
    bool parsed = false;
    // Parse utility flags
    for (int i = 1; i < argc; i++) {
        const char* s = argv[i];
        if (io_args_flags_map.contains(s)) {
            if (i + 1 >= argc)
                invalidArg;
            const char* path = argv[++i];
            io_args_flags_map[s](path);
        } else if (std::strcmp(s, "-h") == 0 || std::strcmp(s, "--help") == 0) {
            PRINT_HELP
            fclose(in);
            in = NULL;
            fclose(out);
            out = NULL;
            return 0;
        }
        parsed = true;
    }
    if (strcmp(input_filepath, output_filepath) == 0) {
        std::cerr << "[!] Input and out put must differ!\n";
        return 2;
    }
    FILE* tmp;
    std::vector<FILE*> to_close = {in, out, table};
    // Parse functional flags
    for (int i = 1; i < argc; i++) {
        const char* s = argv[i];
        if (flags_with_arg_map.contains(s)) {
            if (i + 1 >= argc)
                invalidArg;
            if (in != stdin) {
                call_stack.push({flags_with_arg_map[s], in});
            } else {
                tmp = tmpfile();
                if (tmp == NULL) {
                    std::cerr << "[!] Tmp file creation error!";
                    return 1;
                }
                fprintf(tmp, "%s", argv[++i]);
                call_stack.push({flags_with_arg_map[s], tmp});
            }
        }
        parsed = true;
    }
    if (!parsed) {
        PRINT_HELP
        exit(0);
    }
    if (in == NULL) {
        printf("Input file does not exist!\n");
        exit(1);
    }
    while (!call_stack.empty()) {
        auto p = call_stack.top();
        p.first(p.second, out, table);
        call_stack.pop();
    }
    for (FILE* f : to_close) {
        if (f != nullptr && f != NULL) {
            fclose(f);
        }
    }
    return 0;
}
int main(int argc, const char* argv[]) {
    return flag_parse(argc, argv);
}
