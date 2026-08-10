#include "bst.hpp"

Bst::Bst() : root(nullptr), count(0) {}
Bst::~Bst() { destroy(root); }

void Bst::destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ---------- INSERT ----------
bool Bst::insert(const WordEntry& entry) {
    bool inserted = false;
    root = insertHelper(root, entry, inserted);
    if (inserted) count++;
    return inserted;
}

Bst::Node* Bst::insertHelper(Node* node, const WordEntry& entry, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new Node(entry);
    }
    int cmp = compareEntries(entry, node->data);
    if (cmp < 0) node->left = insertHelper(node->left, entry, inserted);
    else if (cmp > 0) node->right = insertHelper(node->right, entry, inserted);
    // cmp == 0 -> duplicate, do nothing
    return node;
}

// ---------- SEARCH ----------
const WordEntry* Bst::search(int jlptLevel, const std::string& word) const {
    WordEntry key;
    key.jlptLevel = jlptLevel;
    key.word = word;
    const Node* found = findHelper(root, key);
    return found ? &found->data : nullptr;
}

const Bst::Node* Bst::findHelper(const Node* node, const WordEntry& key) const {
    if (node == nullptr) return nullptr;
    int cmp = compareEntries(key, node->data);
    if (cmp == 0) return node;
    if (cmp < 0) return findHelper(node->left, key);
    return findHelper(node->right, key);
}

// ---------- INORDER ----------
std::vector<WordEntry> Bst::inorder() const {
    std::vector<WordEntry> out;
    out.reserve(count);
    inorderHelper(root, out);
    return out;
}

void Bst::inorderHelper(const Node* node, std::vector<WordEntry>& out) const {
    if (node == nullptr) return;
    inorderHelper(node->left, out);
    out.push_back(node->data);
    inorderHelper(node->right, out);
}

// ---------- RANGE QUERY ----------
std::vector<WordEntry> Bst::rangeQuery(int lowLevel, int highLevel) const {
    std::vector<WordEntry> out;
    rangeHelper(root, lowLevel, highLevel, out);
    return out;
}

// Skips whole branches that can't be in range instead of checking every node.
void Bst::rangeHelper(const Node* node, int low, int high, std::vector<WordEntry>& out) const {
    if (node == nullptr) return;
    if (node->data.jlptLevel > high) {
        rangeHelper(node->left, low, high, out);
    } else if (node->data.jlptLevel < low) {
        rangeHelper(node->right, low, high, out);
    } else {
        rangeHelper(node->left, low, high, out);
        out.push_back(node->data);
        rangeHelper(node->right, low, high, out);
    }
}