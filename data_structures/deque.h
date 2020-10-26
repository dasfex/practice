#pragma once

#include <initializer_list>
#include <algorithm>

class Deque {
public:
    Deque() = default;

    Deque(const Deque& rhs) {
        for (size_t i = 0; i < rhs.Size(); ++i) {
            PushBack(rhs[i]);
        }
    }

    Deque(Deque&& rhs) = default;

    explicit Deque(size_t size) {
        Reallocate((size + kBlockSize - 1) / kBlockSize);
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) {
        Reallocate(1);
        for (auto val : list) {
            PushBack(val);
        }
    }

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(block_begin_, rhs.block_begin_);
        std::swap(block_end_, rhs.block_end_);
        std::swap(begin_, rhs.begin_);
        std::swap(end_, rhs.end_);
    }

    void PushBack(int value) {
        if (data_.size() == 0) {
            Reallocate(1);
        }
        data_[block_end_]->data[end_] = value;
        if (end_ + 1 == kBlockSize) {
            if (block_end_ + 1 == data_.size()) {
                Reallocate(data_.size() * 2);
            }
            ++block_end_;
            end_ = 0;
        } else {
            ++end_;
        }
        ++size_;
    }

    void PopBack() {
        if (end_ == 0) {
            --block_end_;
            end_ = kBlockSize - 1;
        } else {
            --end_;
        }
        --size_;
    }

    void PushFront(int value) {
        if (data_.size() == 0) {
            Reallocate(1);
        }
        if (begin_ == 0) {
            if (block_begin_ == 0) {
                Reallocate(data_.size() * 2);
            }
            --block_begin_;
            begin_ = kBlockSize - 1;
        } else {
            --begin_;
        }
        data_[block_begin_]->data[begin_] = value;
        ++size_;
    }

    void PopFront() {
        if (begin_ + 1 == kBlockSize) {
            ++block_begin_;
            begin_ = 0;
        } else {
            ++begin_;
        }
        --size_;
    }

    int& operator[](size_t ind) {
        auto [block, elem] = GetIndex(ind);
        return data_[block]->data[elem];
    }

    int operator[](size_t ind) const {
        auto [block, elem] = GetIndex(ind);
        return data_[block]->data[elem];
    }

    size_t Size() const {
        return size_;
    }

    void Clear() {
        for (auto& block : data_) {
            delete block;
        }
        data_.clear();
        size_ = 0;
        block_begin_ = 0;
        block_end_ = 0;
        begin_ = 0;
        end_ = 0;
    }

    ~Deque() {
        Clear();
    }

private:
    constexpr static size_t kBlockSize = 512 / sizeof(int);

    struct Block {
        int data[kBlockSize];
    };

    std::vector<Block*> data_;
    size_t size_ = 0;
    size_t block_begin_ = 0;
    size_t block_end_ = 0;
    size_t begin_ = 0;
    size_t end_ = 0;

    void Reallocate(size_t new_block_cnt) {
        std::vector<Block*> new_data;
        new_block_cnt -= data_.size();
        size_t left_blocks = new_block_cnt / 2 + 1;
        size_t right_blocks = (new_block_cnt + 1) / 2 + 1;
        for (size_t i = 0; i < left_blocks; ++i) {
            new_data.push_back(new Block);
        }
        for (size_t i = 0; i < data_.size(); ++i) {
            new_data.push_back(data_[i]);
        }
        for (size_t i = 0; i < right_blocks; ++i) {
            new_data.push_back(new Block);
        }
        block_begin_ = left_blocks + block_begin_;
        block_end_ = left_blocks + block_end_;
        data_ = std::move(new_data);
    }

    std::pair<size_t, size_t> GetIndex(size_t ind) const {
        size_t block = 0, elem;
        if (block_begin_ == block_end_) {
            elem = begin_ + ind;
        } else {
            if (ind < kBlockSize - begin_) {
                elem = begin_ + ind;
            } else {
                ind -= kBlockSize - begin_;
                block = ind / kBlockSize + 1;
                elem = ind % kBlockSize;
            }
        }
        return {block_begin_ + block, elem};
    }
};
