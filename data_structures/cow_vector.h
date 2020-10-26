#pragma once

class COWVector {
private:
    struct State {
        State(size_t cp, size_t sz, std::string* data)
            : ref_count(1), capacity(cp), size(sz), data(data) {
        }

        size_t ref_count;
        size_t capacity;
        size_t size;
        std::string* data;
    };

public:
    COWVector() : state_(new State(1, 0, new std::string[1])) {
    }

    ~COWVector() {
        Delete(state_);
    }

    COWVector(const COWVector& other) : state_(other.state_) {
        ++state_->ref_count;
    }

    COWVector& operator=(const COWVector& other) {
        Delete(state_);
        state_ = other.state_;
        ++state_->ref_count;
        return *this;
    }

    size_t Size() const {
        return state_->size;
    }

    void Resize(size_t size) {
        if (state_->size == size) {
            return;
        }
        State* copy = state_;
        state_ = new State(size, size, new std::string[size]);
        for (size_t i = 0; i < std::min(size, copy->size); ++i) {
            state_->data[i] = copy->data[i];
        }
        Delete(copy);
    }

    const std::string& Get(size_t at) {
        return state_->data[at];
    }

    const std::string& Back() {
        return Get(state_->size - 1);
    }

    void PushBack(const std::string& value) {
        State* copy = state_;
        size_t new_cp = copy->size == copy->capacity ? copy->capacity * 2 : copy->capacity;
        state_ = new State(new_cp, copy->size + 1, new std::string[new_cp]);
        for (size_t i = 0; i < copy->size; ++i) {
            state_->data[i] = copy->data[i];
        }
        state_->data[copy->size] = value;
        Delete(copy);
    }

    void Set(size_t at, const std::string& value) {
        if (state_->ref_count != 1) {
            State* copy = state_;
            state_ = new State(copy->size, copy->capacity, new std::string[copy->size]);
            for (size_t i = 0; i < copy->size; ++i) {
                state_->data[i] = copy->data[i];
            }
            state_->data[at] = value;
            --copy->ref_count;
        } else {
            state_->data[at] = value;
        }
    }

private:
    State* state_;

    void Delete(State* state) {
        if (state->ref_count == 1) {
            delete[] state->data;
            delete state;
        } else {
            --state->ref_count;
        }
    }
};