#include "algo.h"
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "tree.h"
using namespace std;
namespace algo {

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

string encode(const char* msg, FILE* out) {
    unordered_set<char> uniques;
    unordered_map<char, int> counter;
    size_t n = strlen(msg);
    for (int i = 0; i < n; i++) {
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
    for (auto i : mp) {
        fprintf(out, "%c -- %s\n", i.first, i.second.c_str());
    }
    return "CRY!";
}
string decode(const char* code, FILE* out) {
    return "ASSASDASDAS";
}
}  // namespace algo
