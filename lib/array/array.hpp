#pragma once

#include <cinttypes>
#include <iostream>

static const size_t kDefaultCapacity = 16;

template <typename T>
class Array {
   private:
    T* data_;
    size_t capacity_;
    size_t size_;
    size_t front_;
    size_t back_;

    class Iterator {
       private:
        size_t index_;
        Array<T>* array_;
        bool is_end_;

       public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(size_t index, Array<T>* array, bool is_end = false)
            : index_(index), array_(array), is_end_(is_end) {}

        Iterator& operator++() { return (*this) += 1; }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--() { return (*this) -= 1; }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        Iterator& operator+=(ssize_t n) {
            if (n < 0) {
                return (*this) -= (-n);
            }
            if (is_end_) {
                return *this;
            }
            if ((index_ < array_->back_ && index_ + n >= array_->back_) ||
                (index_ >= array_->front_ &&
                 (index_ + n) % array_->capacity_ >= array_->back_ &&
                 index_ + n >= array_->capacity_)) {
                index_ = array_->back_;
                is_end_ = true;
                return *this;
            }
            index_ = (index_ + n) % array_->capacity_;
            return *this;
        }

        Iterator operator+(ssize_t n) const {
            Iterator tmp = *this;
            return tmp += n;
        }

        Iterator& operator-=(ssize_t n) {
            if (n <= 0) {
                return (*this) += (-n);
            }
            if (is_end_) {
                is_end_ = false;
                index_ = array_->back_;
            }
            if (index_ < n) {
                index_ = array_->capacity_ - (n - index_);
                return *this;
            }
            index_ = index_ - n;
            return *this;
        }

        Iterator operator-(ssize_t n) const {
            Iterator tmp = *this;
            return tmp -= n;
        }

        difference_type operator-(const Iterator& other) const {
            if (index_ == other.index_ && is_end_ != other.is_end_) {
                return array_->capacity_;
            }
            if (array_->front_ < array_->back_) {
                return index_ - other.index_;
            }
            if ((index_ >= array_->front_ && other.index_ >= array_->front_) ||
                (index_ < array_->back_ && other.index_ < array_->back_) ||
                (index_ <= array_->back_ && other.index_ <= array_->back_ &&
                 is_end_ != other.is_end_)) {
                return index_ - other.index_;
            }
            if (index_ > other.index_) {
                return index_ - other.index_ - array_->capacity_;
            }
            return index_ - other.index_ + array_->capacity_;
        }

        bool operator==(const Iterator& other) const {
            return index_ == other.index_ && is_end_ == other.is_end_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        bool operator<(const Iterator& other) const {
            return (*this - other) < 0;
        }

        bool operator>(const Iterator& other) const { return other < *this; }

        bool operator<=(const Iterator& other) const {
            return !(*this > other);
        }

        bool operator>=(const Iterator& other) const {
            return !(*this < other);
        }

        reference operator*() const { return array_->data_[index_]; }

        pointer operator->() const { return &(array_->data_[index_]); }

        reference operator[](size_t n) const { return *(*this + n); }
    };

   public:
    // Constructors and Destructor:
    Array() : Array(kDefaultCapacity) {}
    Array(size_t new_capacity)
        : capacity_(new_capacity), size_(0), front_(0), back_(0) {
        data_ = new T[capacity_];
    }
    Array(size_t new_size, const T& value)
        : capacity_(new_size),
          size_(new_size),
          front_(0),
          back_(new_size % capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }
    Array(const Array& other)
        : capacity_(other.capacity_),
          size_(other.size_),
          front_(other.front_),
          back_(other.back_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    ~Array() { delete[] data_; }

    // Capacity and Size:
    size_t Capacity() const { return capacity_; }

    bool Empty() const { return size_ == 0; }

    size_t Size() const { return size_; }

    void Resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[(front_ + i) % capacity_];
        }
        front_ = 0;
        back_ = size_ % new_capacity;
        capacity_ = new_capacity;
        delete[] data_;
        data_ = new_data;
    }

    // Modifiers:
    void PushFront(const T& value) {
        if (size_ == capacity_) {
            Resize(capacity_ * 2);
        }
        front_ = (front_ - 1 + capacity_) % capacity_;
        data_[front_] = value;
        ++size_;
    }

    void PushBack(const T& value) {
        if (size_ == capacity_) {
            Resize(capacity_ * 2);
        }
        data_[back_] = value;
        back_ = (back_ + 1) % capacity_;
        ++size_;
    }

    void PopFront() {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    void PopBack() {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        back_ = (back_ - 1 + capacity_) % capacity_;
        --size_;
    }

    // Reference to elements:
    T& At(size_t index_) {
        if (index_ >= size_) {
            throw std::out_of_range("Index is out of range");
        }
        return data_[(front_ + index_) % capacity_];
    }

    const T& At(size_t index_) const {
        if (index_ >= size_) {
            throw std::out_of_range("Index is out of range");
        }
        return data_[(front_ + index_) % capacity_];
    }

    T& Front() {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_[front_];
    }

    const T& Front() const {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_[front_];
    }

    T& Back() {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_[(back_ - 1 + capacity_) % capacity_];
    }

    const T& Back() const {
        if (Empty()) {
            throw std::out_of_range("Array is empty");
        }
        return data_[(back_ - 1 + capacity_) % capacity_];
    }

    T& operator[](size_t index_) {
        return data_[(front_ + index_) % capacity_];
    }

    const T& operator[](size_t index_) const {
        return data_[(front_ + index_) % capacity_];
    }

    Iterator begin() { return Iterator(front_, this); }

    Iterator end() { return Iterator(back_, this, true); }

    // Operators with =:
    Array& operator=(const Array& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        front_ = other.front_;
        back_ = other.back_;
        data_ = new T[capacity_];
        for (size_t i = 0; i < capacity_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    bool operator==(const Array& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[(front_ + i) % capacity_] !=
                other.data_[(other.front_ + i) % other.capacity_]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Array& other) const { return !(*this == other); }
};