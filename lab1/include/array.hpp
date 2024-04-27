
#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <iostream>
#include <concepts>
#include <memory>

template <class T>
concept Arrayable = std::is_default_constructible<T>::value;

template <Arrayable T>
class Array {
public:
    class iterator;

    Array() noexcept;

    Array(size_t size);

    Array(size_t size, T const& t);

    Array(const std::initializer_list<T> &t);

    Array(Array const& other);

    Array(Array &&other) noexcept;

    Array& operator=(Array const& other);

    Array& operator=(Array&& other);

    void copy_content(Array const& other);

    void resize(size_t size);

    void reserve(size_t size);

    void push_back(T const& t);

    void push_back(T&& t);

    T& operator[](size_t index);

    T operator[](size_t index) const;

    size_t size() const noexcept;

    void set_multiplier(uint8_t multiplayer);

    virtual ~Array() noexcept;

protected:
    void copy(Array const& other);
    void move(Array&& other);

    void push_back(T& t);

protected:
    size_t _capacity;
    size_t _size;
    std::shared_ptr<T[]> _array;

public:
    uint8_t const MULTIPLIER = 2;
};

template <Arrayable T> 
void swap(Array<T>& a, Array<T>& b) noexcept;


template <Arrayable T> 
Array<T>::Array() noexcept 
    : _capacity(0), _size(0), _array{nullptr} {}

template <Arrayable T>
Array<T>::Array(size_t size)
    : _capacity(size), _size(_capacity), _array(std::shared_ptr<T[]>(new T[size])) {}

template <Arrayable T>
Array<T>::Array(size_t size, T const& t)
 : Array(size) {
    for (size_t i = 0; i < _size; ++i) {
        _array[i] = t;
    }
}

template <Arrayable T> 
Array<T>::Array(const std::initializer_list<T> &t)
    : Array(t.size()) {
    size_t i{0};

    for (auto &c : t)
        _array[i++] = c;
}

template <Arrayable T>
void Array<T>::copy(const Array<T>& other) {
    _capacity = other._capacity;
    _size = other._size;
    if (other._array) {
        _array = std::shared_ptr<T[]>(new T[_size]);
        copy_content(other);
    }
    set_multiplier(other.MULTIPLIER);
}

template <Arrayable T>
void Array<T>::move(Array<T>&& other) {
    _capacity = other._capacity;
    _size = other._size;
    _array = other._array;
    set_multiplier(other.MULTIPLIER);

    other._capacity = 0;
    other._size = 0;
    other._array = nullptr;
}


template <Arrayable T> 
Array<T>::Array(Array const& other) {
    copy(other);
}

template <Arrayable T> 
Array<T>::Array(Array &&other) noexcept {
    move(std::move(other));
}

template <Arrayable T>
Array<T>& Array<T>::operator=(const Array<T>& other) {
    copy(other);
    return *this;
}

template <Arrayable T>
Array<T>& Array<T>::operator=(Array<T>&& other) {
    move(other);
    return *this;
}

template <Arrayable T>
void Array<T>::copy_content(const Array<T>& other) {
    if (_capacity < other._size) {
        throw std::invalid_argument("Error: trying to copy content of larger array");
    } else {
        for (size_t i = 0; i < other._size; ++i) {
            _array[i] = other._array[i];
        }
        _size = other._size;
    }
}


template <Arrayable T>
void Array<T>::resize(size_t size) {
    if (size <= _capacity) {
        _size = size;
    } else {
        reserve(size);
    }
}

template <Arrayable T>
void Array<T>::reserve(size_t size) {
    if (_capacity < size) {
        Array<T> _arr(size);
        _arr._size = this->_size;
        _arr.set_multiplier(MULTIPLIER);
        _arr.copy_content(*this);
        move(std::move(_arr));
    }
}

template <Arrayable T>
void Array<T>::push_back(T& t) {
    if (_capacity == 0) {
        reserve(1);
        push_back(t);
    } else if (_size < _capacity) {
        _array[_size] = t;
        ++_size;
    } else {
        Array<T> _arr(_capacity * MULTIPLIER);
        _arr.set_multiplier(MULTIPLIER);
        _arr.copy_content(*this);
        move(std::move(_arr));
        push_back(t);
    }
}

template <Arrayable T>
void Array<T>::push_back(T const& t) {
    auto _t = t;
    push_back(static_cast<T&>(_t));
}

template <Arrayable T>
 void Array<T>::push_back(T&& t) {
    push_back(static_cast<T&>(t));
}

template <Arrayable T>
T& Array<T>::operator[](size_t index) {
    if (_size <= index) {
        throw std::invalid_argument("Trying access array's element out of it's bounds.");
    } else {
        return _array[index];
    }
}

template <Arrayable T>
T Array<T>::operator[](size_t index) const {
    return const_cast<Array<T>&>(*this)[index];
}

template <Arrayable T>
size_t Array<T>::size() const noexcept {
    return _size;
}

template <Arrayable T>
void  Array<T>::set_multiplier(uint8_t multiplayer) {
    if (multiplayer < 2) {
        throw std::invalid_argument("Error: trying to set multiplier less than 2");
    }
    *const_cast<uint8_t*>(&MULTIPLIER) = multiplayer;
}


template <Arrayable T>
Array<T>::~Array() noexcept {}


template <Arrayable T> 
void swap(Array<T>& a, Array<T>& b) noexcept {
    auto c = std::move(a);
    a = std::move(b);
    b = std::move(c);
}

#endif