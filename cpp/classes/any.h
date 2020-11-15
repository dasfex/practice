#pragma once

class Any {
private:
    struct WrapperBase {
        using Ptr = std::shared_ptr<WrapperBase>;

        virtual ~WrapperBase() {
        }
    };

    template <class T>
    struct Wrapper : WrapperBase {
        using Type = T;

        Wrapper(T value) : value_(value) {
        }

        const T& GetVal() const {
            return value_;
        }

    private:
        T value_;
    };

public:
    Any() : wrapper_(nullptr) {
    }

    template <class T>
    Any(const T& value) : wrapper_(std::make_shared<Wrapper<T>>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        wrapper_ = std::make_shared<Wrapper<T>>(value);
        return *this;
    }

    Any(const Any& rhs) : wrapper_(rhs.wrapper_) {
    }

    Any& operator=(const Any& rhs) {
        Any tmp(rhs);
        Swap(tmp);
        return *this;
    }

    ~Any() {
    }

    bool Empty() const {
        return wrapper_ == nullptr;
    }

    void Clear() {
        wrapper_ = nullptr;
    }

    void Swap(Any& rhs) {
        std::swap(wrapper_, rhs.wrapper_);
    }

    template <class T>
    const T& GetValue() const {
        auto ptr = std::dynamic_pointer_cast<Wrapper<T>>(wrapper_);
        if (!ptr) {
            throw std::bad_cast();
        }
        return ptr->GetVal();
    }

private:
    typename WrapperBase::Ptr wrapper_;
};
