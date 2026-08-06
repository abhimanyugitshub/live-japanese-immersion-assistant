#include "../hashtable.hpp"
#include <iostream>
#include <string>
#include <cassert>

int testsRun = 0;
int testsPassed = 0;

#define CHECK(cond, name) \
    do { \
        testsRun++; \
        if (cond) { testsPassed++; std::cout << "  [PASS] " << name << "\n"; } \
        else { std::cout << "  [FAIL] " << name << "\n"; } \
    } while (0)

void test_insert_get_roundtrip() {
    std::cout << "test_insert_get_roundtrip\n";
    ds::HashTable<std::string> table;
    table.insert("hello", "world");
    CHECK(table.get("hello") == "world", "basic insert/get roundtrip");
}

void test_japanese_utf8_keys() {
    std::cout << "test_japanese_utf8_keys\n";
    ds::HashTable<std::string> table;
    table.insert("元気", "genki - healthy, energetic");
    table.insert("お元気ですか", "how are you?");
    CHECK(table.get("元気") == "genki - healthy, energetic", "UTF-8 Japanese key insert/get");
    CHECK(table.get("お元気ですか") == "how are you?", "UTF-8 Japanese key with different byte length");
    CHECK(table.contains("元気") == true, "contains() true for existing Japanese key");
}

void test_contains_missing_key() {
    std::cout << "test_contains_missing_key\n";
    ds::HashTable<int> table;
    table.insert("existing", 42);
    CHECK(table.contains("missing") == false, "contains() false for missing key");
}

void test_remove_then_contains() {
    std::cout << "test_remove_then_contains\n";
    ds::HashTable<int> table;
    table.insert("temp", 1);
    CHECK(table.contains("temp") == true, "key present before removal");
    table.remove("temp");
    CHECK(table.contains("temp") == false, "key absent after removal");
    CHECK(table.size() == 0, "size decremented after removal");
}

void test_rehash_preserves_all_entries() {
    std::cout << "test_rehash_preserves_all_entries\n";
    ds::HashTable<int> table(4);

    size_t capacityBefore = table.currentCapacity();
    for (int i = 0; i < 100; ++i) {
        table.insert("key" + std::to_string(i), i);
    }
    size_t capacityAfter = table.currentCapacity();

    CHECK(capacityAfter > capacityBefore, "capacity grew after many inserts");

    bool allPresent = true;
    for (int i = 0; i < 100; ++i) {
        if (!table.contains("key" + std::to_string(i)) || table.get("key" + std::to_string(i)) != i) {
            allPresent = false;
            break;
        }
    }
    CHECK(allPresent, "all 100 entries retrievable after rehash");
    CHECK(table.size() == 100, "size correct after rehash");
}

void test_update_existing_key() {
    std::cout << "test_update_existing_key\n";
    ds::HashTable<int> table;
    table.insert("score", 10);
    table.insert("score", 20);
    CHECK(table.get("score") == 20, "insert on existing key overwrites value");
    CHECK(table.size() == 1, "size doesn't double-count overwritten key");
}

int main() {
    test_insert_get_roundtrip();
    test_japanese_utf8_keys();
    test_contains_missing_key();
    test_remove_then_contains();
    test_rehash_preserves_all_entries();
    test_update_existing_key();

    std::cout << "\n" << testsPassed << " / " << testsRun << " tests passed\n";
    return (testsPassed == testsRun) ? 0 : 1;
}
