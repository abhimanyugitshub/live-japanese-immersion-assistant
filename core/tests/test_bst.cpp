#include <cassert>
#include <iostream>
#include "../bst.hpp"

bool contains(const std::vector<WordEntry>& v, const std::string& word, int level) {
    for (const auto& e : v) if (e.word == word && e.jlptLevel == level) return true;
    return false;
}

void test_insert_and_size() {
    Bst tree;
    assert(tree.empty());
    assert(tree.insert(WordEntry("食べる", 5, "たべる", "to eat")));
    assert(tree.insert(WordEntry("飲む", 5, "のむ", "to drink")));
    assert(tree.insert(WordEntry("経済", 2, "けいざい", "economy")));
    assert(tree.size() == 3);

    // duplicate (same level + word) should be rejected
    assert(!tree.insert(WordEntry("食べる", 5, "たべる", "to eat")));
    assert(tree.size() == 3);

    // same word, different level -> allowed, different entry
    assert(tree.insert(WordEntry("食べる", 4, "たべる", "to eat (N4 variant)")));
    assert(tree.size() == 4);

    std::cout << "test_insert_and_size passed\n";
}

void test_search() {
    Bst tree;
    tree.insert(WordEntry("水", 5, "みず", "water"));
    tree.insert(WordEntry("憂鬱", 1, "ゆううつ", "melancholy"));

    const WordEntry* found = tree.search(5, "水");
    assert(found != nullptr && found->meaning == "water");

    assert(tree.search(3, "存在しない") == nullptr); // not present
    assert(tree.search(3, "水") == nullptr);          // right word, wrong level

    std::cout << "test_search passed\n";
}

void test_inorder_traversal_is_sorted() {
    Bst tree;
    tree.insert(WordEntry("難しい", 3, "むずかしい", "difficult"));
    tree.insert(WordEntry("水", 5, "みず", "water"));
    tree.insert(WordEntry("経済", 2, "けいざい", "economy"));
    tree.insert(WordEntry("あ", 5, "あ", "ah"));
    tree.insert(WordEntry("憂鬱", 1, "ゆううつ", "melancholy"));

    auto sorted = tree.inorder();
    assert(sorted.size() == 5);
    for (size_t i = 1; i < sorted.size(); ++i)
        assert(compareEntries(sorted[i - 1], sorted[i]) < 0);

    assert(sorted[0].jlptLevel == 1);
    assert(sorted[3].jlptLevel == 5 && sorted[3].word == "あ");
    assert(sorted[4].jlptLevel == 5 && sorted[4].word == "水");

    std::cout << "test_inorder_traversal_is_sorted passed\n";
}

void test_range_query() {
    Bst tree;
    tree.insert(WordEntry("食べる", 5, "たべる", "to eat"));
    tree.insert(WordEntry("飲む", 5, "のむ", "to drink"));
    tree.insert(WordEntry("走る", 4, "はしる", "to run"));
    tree.insert(WordEntry("経済", 2, "けいざい", "economy"));
    tree.insert(WordEntry("憂鬱", 1, "ゆううつ", "melancholy"));

    // "every word at this level" -> low == high
    auto n5 = tree.rangeQuery(5, 5);
    assert(n5.size() == 2);
    assert(contains(n5, "食べる", 5));
    assert(!contains(n5, "走る", 4));

    // wider range
    auto mid = tree.rangeQuery(2, 4);
    assert(mid.size() == 2);
    assert(contains(mid, "走る", 4));
    assert(contains(mid, "経済", 2));

    std::cout << "test_range_query passed\n";
}

int main() {
    test_insert_and_size();
    test_search();
    test_inorder_traversal_is_sorted();
    test_range_query();
    std::cout << "ALL TESTS PASSED\n";
    return 0;
}