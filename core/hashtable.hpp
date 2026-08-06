#ifndef DS_HASHTABLE_HPP
#define DS_HASHTABLE_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>

namespace ds {

inline uint64_t fnv1a(const std::string& key) {
    uint64_t hash = 14695981039346656037ULL;
    for (unsigned char c : key) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }
    return hash;
}

template <typename V>
class HashTable {
private:
    struct Entry {
        std::string key;
        V value;
        bool occupied = false;
        bool deleted = false;
    };

    Entry* table;
    size_t capacity;
    size_t count;
    static constexpr double MAX_LOAD_FACTOR = 0.7;

    size_t indexFor(const std::string& key, size_t cap) const {
        return static_cast<size_t>(fnv1a(key) % cap);
    }

    void rehash() {
        size_t oldCapacity = capacity;
        Entry* oldTable = table;

        capacity *= 2;
        table = new Entry[capacity];
        count = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            if (oldTable[i].occupied && !oldTable[i].deleted) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
        delete[] oldTable;
    }

public:
    explicit HashTable(size_t initialCapacity = 16)
        : capacity(initialCapacity), count(0) {
        table = new Entry[capacity];
    }

    ~HashTable() {
        delete[] table;
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void insert(const std::string& key, const V& value) {
        if (static_cast<double>(count + 1) / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t idx = indexFor(key, capacity);
        size_t start = idx;
        while (table[idx].occupied && !table[idx].deleted && table[idx].key != key) {
            idx = (idx + 1) % capacity;
            if (idx == start) throw std::runtime_error("HashTable full");
        }

        if (!table[idx].occupied || table[idx].deleted) {
            count++;
        }
        table[idx].key = key;
        table[idx].value = value;
        table[idx].occupied = true;
        table[idx].deleted = false;
    }

    bool contains(const std::string& key) const {
        size_t idx = indexFor(key, capacity);
        size_t start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) return true;
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        return false;
    }

    V get(const std::string& key) const {
        size_t idx = indexFor(key, capacity);
        size_t start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) return table[idx].value;
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        throw std::out_of_range("Key not found: " + key);
    }

    void remove(const std::string& key) {
        size_t idx = indexFor(key, capacity);
        size_t start = idx;
        while (table[idx].occupied) {
            if (!table[idx].deleted && table[idx].key == key) {
                table[idx].deleted = true;
                count--;
                return;
            }
            idx = (idx + 1) % capacity;
            if (idx == start) break;
        }
        throw std::out_of_range("Key not found: " + key);
    }

    size_t size() const { return count; }
    size_t currentCapacity() const { return capacity; }
};

}

#endif
