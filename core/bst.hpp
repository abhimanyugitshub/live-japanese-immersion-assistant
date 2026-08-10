#pragma once
#include <string>
#include <vector>

// One dictionary word + its JLPT level.
// JLPT levels: 5 = N5 (easiest) ... 1 = N1 (hardest).
struct WordEntry {
    std::string word;
    int jlptLevel;
    std::string reading;
    std::string meaning;

    WordEntry() : jlptLevel(0) {}
    WordEntry(std::string w, int lvl, std::string r, std::string m)
        : word(std::move(w)), jlptLevel(lvl), reading(std::move(r)), meaning(std::move(m)) {}
};

// Compare two entries: first by level, then by word (tiebreaker).
// <0 means a comes first, 0 means same entry, >0 means a comes after.
inline int compareEntries(const WordEntry& a, const WordEntry& b) {
    if (a.jlptLevel != b.jlptLevel) return (a.jlptLevel < b.jlptLevel) ? -1 : 1;
    if (a.word != b.word) return (a.word < b.word) ? -1 : 1;
    return 0;
}

// Binary Search Tree of WordEntry, ordered by JLPT level then word.
class Bst {
public:
    Bst();
    ~Bst();
    Bst(const Bst&) = delete;
    Bst& operator=(const Bst&) = delete;

    bool insert(const WordEntry& entry);
    const WordEntry* search(int jlptLevel, const std::string& word) const;
    std::vector<WordEntry> inorder() const;
    std::vector<WordEntry> rangeQuery(int lowLevel, int highLevel) const;

    int size() const { return count; }
    bool empty() const { return root == nullptr; }

private:
    struct Node {
        WordEntry data;
        Node* left;
        Node* right;
        explicit Node(const WordEntry& d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int count;

    Node* insertHelper(Node* node, const WordEntry& entry, bool& inserted);
    const Node* findHelper(const Node* node, const WordEntry& key) const;
    void inorderHelper(const Node* node, std::vector<WordEntry>& out) const;
    void rangeHelper(const Node* node, int low, int high, std::vector<WordEntry>& out) const;
    void destroy(Node* node);
};