#pragma clang diagnostic ignored "-Wpragma-once-outside-header"
#pragma once

namespace algo {
struct Node {
    char num;
    char sym = '\0';
    Node* left = nullptr;
    Node* right = nullptr;
    Node(char num) : num(num) {}
};
class ShennonTree {
  private:
    void delete_root(Node* root) {
        if (root->left != nullptr) {
            delete_root(root->left);
        }
        if (root->right != nullptr) {
            delete_root(root->right);
        }
        delete root;
    }

  public:
    Node* root = nullptr;
    ShennonTree(Node* root) : root(root){};
    ~ShennonTree() {}
};
}  // namespace algo
