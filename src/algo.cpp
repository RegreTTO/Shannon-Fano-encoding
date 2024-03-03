#include "algo.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "file.h"
#include "tree.h"
using namespace std;
namespace algo {

const string FORMAT = "'%c' -- %s\n";

vector<double> pref_sum;

Node* build_tree(vector<pair<char, double>> p, int start, int end) {
    if (start >= p.size())
        return nullptr;
    Node* root = new Node('\0');
    if (end - 1 - start == 0) {
        root->sym = p[start].first;
        return root;
    }
    int shift = start;
    while (shift + 2 < end &&
           abs((pref_sum[end] - pref_sum[shift + 1]) -
               (pref_sum[shift + 1] - pref_sum[start])) >
               abs((pref_sum[end] - pref_sum[shift + 2]) -
                   (pref_sum[shift + 2] - pref_sum[start]))) {
        shift++;
    }
    root->left = build_tree(p, start, shift + 1);
    root->right = build_tree(p, shift + 1, end);
    if (root->left != nullptr)
        root->left->num = '0';
    if (root->right != nullptr)
        root->right->num = '1';
    return root;
}

map<char, string> dfs(Node* root, string bin = "") {
    if (root == nullptr)
        return {};

    map<char, string> mp;

    if (root->num != '\0')
        bin += root->num;
    if (root->left != nullptr) {
        auto tmp = dfs(root->left, bin);
        if (!tmp.empty()) {
            mp.insert(tmp.begin(), tmp.end());
        }
    }
    if (root->right != nullptr) {
        auto tmp = dfs(root->right, bin);
        if (!tmp.empty()) {
            mp.insert(tmp.begin(), tmp.end());
        }
    }
    if (root->sym != '\0') {
        mp[root->sym] = bin;
    }
    return mp;
}

void encode(FILE* in, FILE* out, FILE* table) {
    unordered_set<char> uniques;
    unordered_map<char, int> counter;
    const char* msg = file::read_file(in);
    size_t n = strlen(msg);
    for (int i = 0; i < n; i++) {
        if (!isprint(msg[i]))
            continue;
        counter[msg[i]]++;
        uniques.emplace(msg[i]);
    }
    vector<pair<char, double>> p;
    pref_sum.resize(uniques.size() + 1);
    for (auto a : counter) {
        p.push_back({a.first, (double)a.second / n});
    }
    sort(p.begin(), p.end(),
         [](auto a1, auto a2) { return a1.second > a2.second; });
    for (int i = 0; i < p.size(); i++) {
        pref_sum[i + 1] = pref_sum[i] + p[i].second;
    }
    ShennonTree tree(build_tree(p, 0, p.size()));
    auto mp = dfs(tree.root);
    if (fileno(out) == fileno(table))
        fprintf(table, "CORRESPONDENCE TABLE\n");
    for (auto i : mp) {
        fprintf(table, FORMAT.c_str(), i.first, i.second.c_str());
    }
    for (int i = 0; i < n; i++) {
        fprintf(out, "%s", mp[msg[i]].c_str());
    }
    return;
}
void decode(FILE* in, FILE* out, FILE* table) {
    if (fileno(table) == fileno(stdout) || fileno(table) == fileno(stdin) ||
        fileno(table) == fileno(stderr)) {
        std::cerr << "[!] You must specify table file when using decode!";
        exit(2);
    }
    char ch;
    char* code;
    unordered_map<string, char> cons_table;
    while (fscanf(table, FORMAT.c_str(), &ch, code) > 1) {
        cons_table[code] = ch;
    }
    if (cons_table.empty()) {
        std::cerr << "[!] Invalid table! Format: \"" + FORMAT + '\"';
    }
    const char* binary = file::read_file(in);
    size_t n = strlen(binary);
    string msg = "";
    string prefix = "";
    for (int i = 0; i < n; i++) {
        prefix += binary[i];
        if (cons_table.contains(prefix)) {
            msg += cons_table[prefix];
            prefix = "";
        }
    }
    fprintf(out, "%s", msg.c_str());

    return;
}
}  // namespace algo
