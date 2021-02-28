#pragma once

#include <unordered_map>
#include <mutex>
#include <functional>

template <class K, class V, class Hash = std::hash<K>>
class ConcurrentUnorderedMap {
public:
    ConcurrentUnorderedMap(const Hash& hasher = Hash())
        : ConcurrentUnorderedMap(kUndefinedSize, hasher) {
    }

    explicit ConcurrentUnorderedMap(int expected_size, const Hash& hasher = Hash())
        : ConcurrentUnorderedMap(expected_size, kDefaultConcurrencyLevel, hasher) {
    }

    ConcurrentUnorderedMap(
        int expected_size,
        int expected_threads_count,
        const Hash& hasher = Hash())
        : hasher_(hasher)
        , mutexes_(std::vector<std::mutex>(expected_threads_count)) {
        if (expected_size != kUndefinedSize) {
            table_size_ = (expected_size + expected_threads_count - 1) / expected_threads_count;
        }
        tables_.reserve(expected_threads_count);
        for (size_t i = 0; i < static_cast<size_t>(expected_threads_count); ++i) {
            tables_.emplace_back(table_size_, hasher);
        }
    }

    bool Insert(const K& key, const V& value) {
        size_t ind = GetTableIndex(key);
        std::lock_guard<std::mutex> lock(mutexes_[ind]);
        auto insert_pair = tables_[ind].emplace(key, value);
        return insert_pair.second;
    }

    bool Erase(const K& key) {
        size_t ind = GetTableIndex(key);
        std::lock_guard<std::mutex> lock(mutexes_[ind]);
        return tables_[ind].erase(key);
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (size_t i = 0; i < tables_.size(); ++i) {
            std::lock_guard<std::mutex> locki(mutexes_[i]);
            tables_[i].clear();
        }
    }

    std::pair<bool, V> Find(const K& key) const {
        size_t ind = GetTableIndex(key);
        std::lock_guard<std::mutex> lock(mutexes_[ind]);
        auto it = tables_[ind].find(key);
        return it == tables_[ind].end() ? std::make_pair(false, V())
                                        : std::make_pair(true, it->second);
    }

    const V At(const K& key) const {
        size_t ind = GetTableIndex(key);
        std::lock_guard<std::mutex> lock(mutexes_[ind]);
        return tables_[ind].at(key);
    }

    size_t Size() const {
        size_t size = 0;
        for (size_t i = 0; i < tables_.size(); ++i) {
            std::lock_guard<std::mutex> lock(mutexes_[i]);
            size += tables_[i].size();
        }
        return size;
    }

    static const int kDefaultConcurrencyLevel;
    static const int kUndefinedSize;

private:
    std::vector<std::unordered_map<K, V, Hash>> tables_;
    size_t table_size_ = 1;
    Hash hasher_;
    mutable std::vector<std::mutex> mutexes_;
    mutable std::mutex mutex_;

    size_t GetTableIndex(const K& key) const {
        size_t hash = hasher_(key) % (table_size_ * tables_.size());
        size_t ind = (hash + 1) / table_size_;
        return ind == 0 ?  ind : ind - 1;
    }
};

template <class K, class V, class Hash>
const int ConcurrentUnorderedMap<K, V, Hash>::kDefaultConcurrencyLevel = 8;

template <class K, class V, class Hash>
const int ConcurrentUnorderedMap<K, V, Hash>::kUndefinedSize = -1;
