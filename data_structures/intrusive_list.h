#pragma once

#include <algorithm>

// your type should inherit ListHook
class ListHook {
public:
    ListHook() : prev_(nullptr), next_(nullptr) {
    }

    bool IsLinked() const {
        return prev_ != nullptr && next_ != nullptr;
    }

    void Unlink() {
        ListHook* prev = prev_;
        ListHook* next = next_;
        if (prev) {
            prev->next_ = next;
        }
        if (next) {
            next->prev_ = prev;
        }
        prev_ = nullptr;
        next_ = nullptr;
    }

    // Must unlink element from list
    ~ListHook() {
        Unlink();
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    ListHook* prev_;
    ListHook* next_;

    // that helper function might be useful
    void LinkAfter(ListHook* other) {
        if (next_ == nullptr && prev_ == nullptr &&
            other->next_ == nullptr & other->prev_ == nullptr) {
            next_ = other;
            other->prev_ = this;
        } else {
            ListHook* next = next_;
            other->prev_ = this;
            other->next_ = next;
            next->prev_ = other;
            next_ = other;
        }
    }
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(ListHook* node) : node_(node) {
        }

        Iterator& operator++() {
            node_ = node_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            node_ = node_->next_;
            return tmp;
        }

        T& operator*() const {
            return *static_cast<T*>(node_);
        }

        T* operator->() const {
            return static_cast<T*>(node_);
        }

        bool operator==(const Iterator& rhs) const {
            return node_ == rhs.node_;
        }

        bool operator!=(const Iterator& rhs) const {
            return !this->operator==(rhs);
        }

    private:
        ListHook* node_;
    };

    List() {
        fbegin_.LinkAfter(&fend_);
    }

    List(const List&) = delete;

    List(List&& other) {
        fbegin_.next_ = other.fbegin_.next_;
        fend_.prev_ = other.fend_.prev_;
        other.fbegin_ = ListHook();
        other.fend_ = ListHook();
        other.fbegin_.LinkAfter(&other.fend_);
    }

    // must unlink all elements from list
    ~List() {
        while (!IsEmpty()) {
            fbegin_.next_->Unlink();
        }
    }

    List& operator=(const List&) = delete;

    List& operator=(List&& other) {
        if (this == &other) {
            return *this;
        }
        this->~List();
        new (this) List(std::move(other));
        return *this;
    }

    bool IsEmpty() const {
        return fbegin_.next_ == &fend_;
    }

    // that method is allowed to be O(n)
    size_t Size() const {
        size_t size = 0;
        ListHook* cur = fbegin_.next_;
        while (cur != &fend_) {
            cur = cur->next_;
            ++size;
        }
        return size;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        fend_.prev_->LinkAfter(elem);
    }

    void PushFront(T* elem) {
        fbegin_.LinkAfter(elem);
    }

    T& Front() {
        return *static_cast<T*>(fbegin_.next_);
    }

    const T& Front() const {
        return *static_cast<T*>(fbegin_.next_);
    }

    T& Back() {
        return *static_cast<T*>(fend_.prev_);
    }

    const T& Back() const {
        return *static_cast<T*>(fend_.prev_);
    }

    void PopBack() {
        fend_.prev_->Unlink();
    }

    void PopFront() {
        fbegin_.next_->Unlink();
    }

    Iterator Begin() {
        return Iterator(fbegin_.next_);
    }

    Iterator End() {
        return Iterator(&fend_);
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        ListHook* node = static_cast<T*>(element);
        return Iterator(node);
    }

private:
    ListHook fbegin_ = ListHook();
    ListHook fend_ = ListHook();
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}
