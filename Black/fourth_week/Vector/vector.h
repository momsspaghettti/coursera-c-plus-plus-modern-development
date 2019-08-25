// #pragma once
#include <cstddef>
#include <new>
#include <utility>
#include <memory>


template<typename T>
struct RawMemory {
public:
    RawMemory() = default;

    explicit RawMemory(size_t n) {
        buf_ = Allocate(n + 1);
        capacity_ = n;
    }

    RawMemory(const RawMemory &) = delete;

    RawMemory(RawMemory &&other) noexcept {
        Swap(other);
    }

    RawMemory &operator=(const RawMemory &) = delete;

    RawMemory &operator=(RawMemory &&other) noexcept {
        Swap(other);
        return *this;
    }

    ~RawMemory() {
        Deallocate(buf_);
        capacity_ = 0;
    }

    T *operator+(size_t i) {
        return buf_ + i;
    }
    const T *operator+(size_t i) const {
        return buf_ + i;
    }

    T &operator[](size_t index) {
        return buf_[index];
    }
    const T &operator[](size_t index) const {
        return buf_[index];
    }

    [[nodiscard]] size_t Capacity() const {
        return capacity_;
    }

    void Swap(RawMemory &other) {
        std::swap(buf_, other.buf_);
        std::swap(capacity_, other.capacity_);
    }

    const T* Buf() const {
        return buf_;
    }
    T* Buf() {
        return buf_;
    }

    const T* Begin() const {
        return buf_ + 0;
    }
    T* Begin() {
        return buf_ + 0;
    }

private:
    T *buf_ = nullptr;
    size_t capacity_ = 0;

    static T *Allocate(size_t n) {
        return static_cast<T *>(operator new(n * sizeof(T)));
    }

    static void Deallocate(T *buf) {
        operator delete(buf);
    }
};


template<typename T>
class Vector {
public:
    Vector() = default;

    explicit Vector(size_t n) : data_(n) {
        std::uninitialized_value_construct_n(
                data_.Buf(), n
                );
        size_ = n;
    }

    Vector(const Vector &other) : data_(other.size_) {
        std::uninitialized_copy_n(
                other.data_.Buf(),
                other.size_,
                data_.Buf()
                );
        size_ = other.size_;
    }

    void Swap(Vector &other) {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
    }

    Vector(Vector &&other) noexcept : data_(other.size_) {
        Swap(other);
    }

    ~Vector() {
        std::destroy_n(data_.Buf(), size_);
    }

    Vector &operator=(const Vector &other) {
        if (other.size_ > data_.Capacity()) {
            Vector<T> tmp(other);
            Swap(tmp);
        } else {
            for (size_t i = 0; i < size_ && i < other.size_; ++i) {
                data_[i] = other.data_[i];
            }
            if (size_ < other.size_) {
                std::uninitialized_copy_n(
                        other.data_.Buf() + size_,
                        other.size_ - size_,
                        data_.Buf() + size_
                );
            } else if (size_ > other.size_) {
                std::destroy_n(
                        data_.Buf() + other.size_,
                        size_ - other.size_
                );
            }
            size_ = other.size_;
        }
        return *this;
    }

    Vector &operator=(Vector &&other) noexcept {
        Swap(other);
        return *this;
    }

    void Reserve(size_t n) {
        if (n > data_.Capacity()) {
            RawMemory<T> data2(n);
            std::uninitialized_move_n(
                    data_.Buf(), size_, data2.Buf()
                    );
            std::destroy_n(data_.Buf(), size_);
            data_ = std::move(data2);
        }
    }

    void Resize(size_t n) {
        Reserve(n);
        if (size_ < n) {
            std::uninitialized_value_construct_n(
                    data_ + size_,
                    n - size_
            );
        } else if (size_ > n) {
            std::destroy_n(
                    data_ + n,
                    size_ - n
            );
        }
        size_ = n;
    }

    void PushBack(const T &elem) {
        if (size_ == data_.Capacity()) {
            Reserve(size_ == 0 ? 1 : 2 * size_);
        }
        new(data_ + size_) T(elem);
        ++size_;
    }

    void PushBack(T &&elem) {
        if (size_ == data_.Capacity()) {
            Reserve(size_ == 0 ? 1 : 2 * size_);
        }
        new(data_ + size_) T(std::move(elem));
        ++size_;
    }

    template<typename ... Args>
    T &EmplaceBack(Args &&... args) {
        if (size_ == data_.Capacity()) {
            Reserve(size_ == 0 ? 1 : 2 * size_);
        }
        auto elem = new(data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
        return *elem;
    }

    void PopBack() {
        std::destroy_at(data_ + size_ - 1);
        --size_;
    }

    [[nodiscard]] size_t Size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t Capacity() const noexcept {
        return data_.Capacity();
    }

    const T &operator[](size_t i) const {
        return data_[i];
    }

    T &operator[](size_t i) {
        return data_[i];
    }

    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept {
        return data_.Begin();
    }

    iterator end() noexcept {
        return data_ + size_;
    }

    const_iterator begin() const noexcept {
        return data_.Begin();
    }

    const_iterator end() const noexcept {
        return data_ + size_;
    }

    // Тут должна быть такая же реализация, как и для константных версий begin/end
    const_iterator cbegin() const noexcept {
        return data_.Begin();
    }

    const_iterator cend() const noexcept {
        return data_ + size_;
    }

    // Вставляет элемент перед pos
    // Возвращает итератор на вставленный элемент
    iterator Insert(const_iterator pos, const T& elem) {
        const size_t offset = pos - data_.Buf();
        RawMemory<T> data2(size_ == Capacity() ? (size_ == 0 ? 1 : 2 * size_) : Capacity());
        std::uninitialized_move_n(
                data_.Buf(),
                offset,
                data2.Buf()
                );
        new (data2 + offset) T(elem);
        if (size_ > offset) {
            std::uninitialized_move_n(
                    data_.Buf() + offset,
                    size_ - offset,
                    data2.Buf() + offset + 1
                    );
        }
        data_.Swap(data2);
        ++size_;
        return begin() + offset;
    }

    iterator Insert(const_iterator pos, T&& elem) {
        const size_t offset = pos - data_.Buf();
        RawMemory<T> data2(size_ == Capacity() ? (size_ == 0 ? 1 : 2 * size_) : Capacity());
        std::uninitialized_move_n(
                data_.Buf(),
                offset,
                data2.Buf()
        );
        new (data2 + offset) T(std::move(elem));
        if (size_ > offset) {
            std::uninitialized_move_n(
                    data_.Buf() + offset,
                    size_ - offset,
                    data2.Buf() + offset + 1
            );
        }
        data_.Swap(data2);
        ++size_;
        return begin() + offset;
    }

    // Конструирует элемент по заданным аргументам конструктора перед pos
    // Возвращает итератор на вставленный элемент
    template <typename ... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        return Insert(pos, T(std::forward<Args>(args)...));
    }

    // Удаляет элемент на позиции pos
    // Возвращает итератор на элемент, следующий за удалённым
    iterator Erase(const_iterator pos) {
        const size_t offset = pos - data_.Buf();
        RawMemory<T> data2(Capacity());
        std::uninitialized_move_n(
                data_.Buf(),
                offset,
                data2.Buf()
        );
        if (size_ > offset) {
            std::uninitialized_move_n(
                    data_.Buf() + offset + 1,
                    size_ - offset,
                    data2.Buf() + offset
            );
        }
        data_.Swap(data2);
        --size_;
        return begin() + offset;
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;
};


void TestVector();