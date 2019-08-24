#include <utility>


// Исключение этого типа должно генерироваться при обращении к "пустому" Optional в функции Value
struct BadOptionalAccess {
};


template<typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)]{};
    bool defined = false;

public:
    Optional() = default;

    Optional(const T &elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T &&elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    static void Assign(Optional* target, const Optional &source) {
        if (target->defined) {
            if (source.defined) {
                *target->Get() = *source.Get();
            } else {
                reinterpret_cast<T*>(target->data)->~T();
            }
        } else {
            if (source.defined)
                new (target->data) T(*source.Get());
        }
        target->defined = source.defined;
    }

    static void Assign(Optional* target, Optional &&source) {
        if (target->defined) {
            if (source.defined) {
                *target->Get() = std::move(*source.Get());
            } else {
                reinterpret_cast<T*>(target->data)->~T();
            }
        } else {
            if (source.defined)
                new (target->data) T(std::move(*source.Get()));
        }
        target->defined = source.defined;
    }

    Optional(const Optional &other) {
        Assign(this, other);
    }

    Optional(Optional &&other) {
        Assign(this, std::move(other));
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            *reinterpret_cast<T*>(data) = elem;
        } else {
            new (data) T(elem);
        }
        defined = true;
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            *reinterpret_cast<T*>(data) = std::move(elem);
        } else {
            new (data) T(std::move(elem));
        }
        defined = true;
        return *this;
    }

    Optional& operator=(const Optional& other) {
        Assign(this, other);
        return *this;
    }

    Optional& operator=(Optional&& other) {
        Assign(this, std::move(other));
        return *this;
    }

    [[nodiscard]] bool HasValue() const {
        return defined;
    }

    // Эти операторы не должны делать никаких проверок на пустоту.
    // Проверки остаются на совести программиста.
    T* Get() {
        return reinterpret_cast<T*>(&data);
    }

    const T* Get() const {
        return reinterpret_cast<const T*>(&data);
    }

    T &operator*() {
        return *Get();
    }

    const T &operator*() const {
        return *Get();
    }

    T *operator->() {
        return Get();
    }

    const T *operator->() const {
        return Get();
    }

    // Генерирует исключение BadOptionalAccess, если объекта нет
    T &Value() {
        if (defined) {
            return *Get();
        } else {
            throw BadOptionalAccess{};
        }
    }

    const T &Value() const {
        if (defined) {
            return *Get();
        } else {
            throw BadOptionalAccess{};
        }
    }

    void Reset() {
        if (defined)
            reinterpret_cast<T*>(data)->~T();
        defined = false;
    }

    ~Optional() {
        if (defined)
            reinterpret_cast<T*>(data)->~T();
        defined = false;
    }
};


void TestOptional();