#pragma once

#include <list>
#include <string>
#include <unordered_map>

class LruCache {
public:
    LruCache(size_t max_size) : max_size_(max_size) {
    }

    void Set(const std::string& key, const std::string& value) {
        if (map_.find(key) != std::end(map_)) {
            list_.erase(map_[key]);
        }
        list_.insert(std::begin(list_), {key, value});
        map_[key] = std::begin(list_);
        if (std::size(list_) > max_size_) {
            map_.erase(std::rbegin(list_)->first);
            auto it = std::end(list_);
            list_.erase(--it);
        }
    }

    bool Get(const std::string& key, std::string* value) {
        if (map_.find(key) == std::end(map_)) {
            return false;
        }
        *value = map_[key]->second;
        list_.erase(map_[key]);
        list_.insert(std::begin(list_), {key, *value});
        map_[key] = std::begin(list_);
        return true;
    }

private:
    size_t max_size_;
    std::list<std::pair<std::string, std::string>> list_;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> map_;
};
