#pragma once

#include <string>

class WeakPtr;

struct State {
    std::string* str;
    size_t count_strong;
    size_t count_weak;

    State(std::string* str = nullptr, size_t count_strong = 1, size_t count_weak = 0)
        : str(str), count_strong(count_strong), count_weak(count_weak) {
    }

    ~State() {
        if (str) {
            delete str;
        }
    }
};

class SharedPtr {
public:
    SharedPtr() : state_(new State()) {
    }

    SharedPtr(const SharedPtr& ptr) : state_(ptr.state_) {
        state_->count_strong += 1;
    }

    SharedPtr(SharedPtr&& ptr) : state_(ptr.state_) {
        ptr.state_ = nullptr;
    }

    SharedPtr(std::string* ptr) : state_(new State(ptr)) {
    }

    SharedPtr& operator=(const SharedPtr& ptr) {
        if (this == &ptr) {
            return *this;
        }
        this->~SharedPtr();
        new (this) SharedPtr(ptr);
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& ptr) {
        if (this == &ptr) {
            return *this;
        }
        this->~SharedPtr();
        new (this) SharedPtr(ptr);  // std::move?
        return *this;
    }

    ~SharedPtr() {
        Delete(state_);
    }

    std::string* Get() const {
        return state_->str;
    }

    std::string& operator*() const {
        return *state_->str;
    }

    void Reset(std::string* new_ptr) {
        this->~SharedPtr();
        new (this) SharedPtr(new_ptr);
    }

    SharedPtr(const WeakPtr& rhs);

    friend WeakPtr;

private:
    State* state_;

    SharedPtr(State* state) : state_(state) {
        state_->count_strong += 1;
    }

    void Delete(State* state) {
        if (state == nullptr) {
            return;
        }
        state->count_strong -= 1;
        if (state->count_strong == 0 && state->count_weak == 0) {
            delete state;
        } else if (state->count_strong == 0) {
            delete state->str;
            state->str = nullptr;
            state = nullptr;
        }
    }
};

class WeakPtr {
public:
    WeakPtr() : state_(new State(nullptr, 0, 1)) {
    }

    WeakPtr(const WeakPtr& ptr) : state_(ptr.state_) {
        state_->count_weak += 1;
    }

    WeakPtr(WeakPtr&& ptr) : state_(ptr.state_) {
        ptr.state_ = nullptr;
    }

    WeakPtr(const SharedPtr& shared) : state_(shared.state_) {
        state_->count_weak += 1;
    }

    WeakPtr& operator=(const WeakPtr& ptr) {
        if (this == &ptr) {
            return *this;
        }
        this->~WeakPtr();
        new (this) WeakPtr(ptr);
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& ptr) {
        if (this == &ptr) {
            return *this;
        }
        this->~WeakPtr();
        new (this) WeakPtr(ptr);  // std::move?
        return *this;
    }

    ~WeakPtr() {
        Delete(state_);
    }

    SharedPtr Lock() const {
        if (state_ == nullptr || state_->count_strong < 1) {
            return SharedPtr();
        }
        return SharedPtr(*this);
    }

    bool IsExpired() const {
        return state_ == nullptr || state_->count_strong < 1;
    }

    friend SharedPtr;

private:
    State* state_;

    void Delete(State* state) {
        if (state == nullptr) {
            return;
        }
        state->count_weak -= 1;
        if (state->count_strong == 0 && state->count_weak == 0) {
            delete state;
        }
    }
};

SharedPtr::SharedPtr(const WeakPtr& rhs) : SharedPtr(rhs.state_) {
}
