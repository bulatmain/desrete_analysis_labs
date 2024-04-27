#include <iostream>
#include <cinttypes>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "array.hpp"

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
