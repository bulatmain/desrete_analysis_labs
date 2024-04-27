
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

#include <iostream>
#include <cinttypes>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

namespace da_lab1 {

#define READER
#ifdef  READER
    #define STRING_SIZE 64

    using id_t = uint16_t;
    using string = std::string;

    template <typename T>
    concept Stream = std::is_convertible_v<T&, std::basic_istream<char>&>;

    struct var {
    public:
        id_t id;
        string val;
    
    public:
        var() : id{}, val{} {}
        var(uint16_t id, string const& val)
            : id{id}, val{val} {}
        var(uint16_t id, string&& val)
            : id{id}, val(std::move(val)) {}
        var(std::pair<id_t, string> const& pr)
            : var(pr.first, pr.second) {}
        var(std::pair<id_t, string>&& pr)
            : var(pr.first, std::move(pr.second)) {}
        var(var const& _v) 
            : var(_v.id, _v.val) {}
        var(var&& _v) 
            : var(_v.id, std::move(_v.val)) {}

        std::pair<id_t, string> toPair() const {
            return std::pair<id_t, string>(id, val);
        }

        string toString() const {
            return std::to_string(static_cast<int>(id)) + "\t" + val;
        }

        static string toString(uint16_t id, std::string const& val) {
            return var(id, val).toString();
        }

        var& operator=(var const& _v) {
            copy(_v);
            return *this;
        }

        var& operator=(var&& _v) {
            move(std::move(_v));
            return *this;
        }

        bool operator==(var const& _v) const {
            return !(*this != _v); 
        }

        bool operator!=(var const& _v) const {
            return this->id != _v.id || this->val != _v.val;
        }
    private:
        void copy(var const& _v) {
            this->id = _v.id;
            this->val = _v.val;
        }

        void move(var&& _v) {
            this->id = _v.id;
            this->val =std::move( _v.val);
        }

    };

    bool operator<(var const& a, var const& b) {
        return a.id < b.id;
    }

    bool operator<=(var const& a, var const& b) {
        return (a < b) || (a == b);
    }

    template <Stream stream>
    void readStringStringSize(stream& s, string& str) {
        std::getline(s, str);
        if (64 < str.size()) {
            throw std::invalid_argument("Error: inputed string with size more than 64");
        } else {
            uint8_t str_size = static_cast<uint8_t>(str.size());
            uint8_t additionCharsCount = STRING_SIZE - str_size;
            str += string(additionCharsCount, '\0');
        }
    }

    template <Stream stream>
    stream& operator>>(stream& s, var& v) {
        s >> v.id;
        s.get();
        readStringStringSize(s, v.val);
        return s;
    }

    std::ostream& operator<<(std::ostream& os, var const& v) {
        os << v.toString();
        return os;
    }

    using var_map = Array<var>;

    void printMap(var_map const& vm) {
        for (size_t i = 0; i < vm.size(); ++i) {
            printf("%d\t%s\n", vm[i].id, vm[i].val.c_str());
        }
    }

    enum ReadingProperties {
        SET_STDIN_SYNC_OFF,
        NONE
    };

    template <Stream stream>
    class MapReader {
    protected:
        ReadingProperties properties;

        stream& s;

        var_map _map;

    public:
        explicit MapReader(ReadingProperties propreties, stream& s) 
            : properties(propreties), s(s) {
            setReadingProperties(SET_STDIN_SYNC_OFF);
        }

        var_map&& read() {
            while (stdinThreadIsNotEmpty()) {
                readAndInsertVar();
            }
            return std::move(_map);
        }
        

    protected:
        void setReadingProperties(ReadingProperties propreties) {
            if (propreties == SET_STDIN_SYNC_OFF) {
                setSTDIOSyncOff();
            }
        }

        void setSTDIOSyncOff() {
            if (std::is_same<stream, std::istream>::value) {
                std::ios_base::sync_with_stdio(false);
                s.tie();
            }
        }

        bool stdinThreadIsNotEmpty() {
            return !s.eof();
        }

        template <class T>
        static stream& readFromStream(stream& s, T& t) {
            s >> t;
            return s;
        }

        void readAndInsertVar() {
            var v; 
            if (readFromStream(s, v)) {
                _map.push_back(std::move(v));
            }
        }

    };


    template <Stream stream>
    var_map readMap(stream& s) {
        MapReader reader(ReadingProperties::SET_STDIN_SYNC_OFF, s);
        return std::move(reader.read());
    }

#endif

#define COUNTING_SORTER
#ifdef  COUNTING_SORTER

class CountingSorter {
protected:
    CountingSorter() = delete;

public:
    static var_map sort(var_map&& _unsortedMap) {
        auto unsortedMap = std::move(_unsortedMap);
        auto maxValue = findMaxValue(unsortedMap);
        auto countArray = countValues(unsortedMap, maxValue);
        sumUp(countArray);
        return post(unsortedMap, countArray);   
    }

protected:
    static uint16_t findMaxValue(var_map const& unsortedMap) {
        uint16_t res = 0;
        for (size_t i = 0; i < unsortedMap.size(); ++i) {
            if (res < unsortedMap[i].id) {
                res = unsortedMap[i].id;
            }
        }
        return res;
    }

    static std::vector<uint64_t> countValues(var_map const& unsortedMap, uint16_t maxValue) {
        std::vector<uint64_t> countArray(maxValue + 1);
        for (size_t i = 0; i < unsortedMap.size(); ++i) {
            ++countArray[unsortedMap[i].id];
        }
        return countArray;
    }

    static void sumUp(std::vector<uint64_t>& countArray) {
        for (size_t i = 1; i < countArray.size(); ++i) {
            countArray[i] += countArray[i - 1];
        }
    }

    static var_map post(var_map& unsortedMap, std::vector<uint64_t>& countArray) {
        var_map sortedMap(unsortedMap.size());
        for (uint64_t i = 0; i < unsortedMap.size(); ++i) {
            uint64_t i_backorder = unsortedMap.size() - i - 1;
            uint16_t j = unsortedMap[i_backorder].id;
            if (countArray[j] != 0) {
                --countArray[j];
                sortedMap[countArray[j]] = std::move(unsortedMap[i_backorder]);
            }
        }
        return sortedMap;
    }
};

}

#endif


#include <sstream>

using namespace da_lab1;

int main() {
    auto unsorted_map = readMap(std::cin);

    auto sorted_map = CountingSorter::sort(std::move(unsorted_map));

    printMap(sorted_map);

    return 0;
}