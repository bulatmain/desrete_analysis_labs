#include <gtest/gtest.h>

#include <lib.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace da_lab1;

class MapReaderTest : public MapReader<std::istringstream> {
public:
    MapReader* fromSuper(MapReaderTest* mrt) {
        return static_cast<MapReader*>(mrt);
    }

    MapReaderTest(std::istringstream& iss)
        : MapReader(ReadingProperties::SET_STDIN_SYNC_OFF, iss) {}

    void _readAndInsertVar() {
        readAndInsertVar();
    }

    var_map& get() {
        return _map;
    }

};

void testReadVarWithInput(uint16_t id, std::string const& val) {
    std::string input = var::toString(id, val);

    var expected_result(id, val);

    std::istringstream iss(input);

    var happened_var; iss >> happened_var;
    EXPECT_TRUE(iss.eof());

    ASSERT_EQ(happened_var, expected_result);
    
}

TEST(MapReaderTests, readVarTest1) {
    uint16_t id = 0;
    std::string val = std::string(64, 'a');
    testReadVarWithInput(id, val);
}

TEST(MapReaderTests, readVarTest2) {
    uint16_t id = 0;
    std::string val = "0123456789" + std::string(54, '\0');
    testReadVarWithInput(id, val);
}

TEST(MapReaderTests, readVarTest3) {
    uint16_t id = 0;
    std::string val = "012 345 678 9" + std::string(51, '\0');
    testReadVarWithInput(id, val);
}

TEST(MapReaderTests, readAndInsertVarTest) {
    uint16_t id = 0;
    std::string val = "0123456789" + std::string(54, '\0');

    var_map expected_result;
    expected_result.push_back(var(id, val));

    std::string input = var::toString(id, val);

    std::istringstream iss(input);
    MapReaderTest reader(iss);

    reader._readAndInsertVar();

    auto happened_result = reader.get();

    ASSERT_EQ(happened_result, expected_result);
}

uint8_t uIntLength(uint64_t i) {
    if (i == 0) {
        return 1;
    } else {
        return static_cast<uint8_t>(log10(static_cast<double>(i))) + 1;
    }
}

TEST(MapReaderTests, readTest) {
    std::string input;
    var_map expected_result;

    for (uint16_t i = 0; i <= 1001; ++i) {
        var v(i, std::to_string(static_cast<int>(i)) + std::string(STRING_SIZE - uIntLength(i), '\0'));
        input += v.toString() + "\n";
        expected_result.push_back(v);
    }

    std::istringstream iss(input);
    MapReaderTest reader(iss);

    auto happened_result = reader.read();

    ASSERT_EQ(happened_result, expected_result);
}

class CountingSorterTest : public CountingSorter {
public:
    static uint16_t _findMaxValue(var_map const& unsortedMap) {
        return findMaxValue(unsortedMap);
    }

    static std::vector<uint64_t> _countValues(var_map const& unsortedMap, uint16_t maxValue) {
        return countValues(unsortedMap, maxValue);
    }

    static std::vector<uint64_t> _sumUp(std::vector<uint64_t> const& countArray) {
        auto _countArray = countArray;
        sumUp(_countArray);
        return _countArray;
    }

    static var_map _post(var_map const& _unsortedMap, std::vector<uint64_t> const& _countArray) {
        auto unsortedMap = _unsortedMap;
        auto countArray  = _countArray;
        return post(unsortedMap, countArray);
    }

    static var_map _sort(var_map const& _unsortedMap) {
        auto unsortedMap = _unsortedMap;
        return sort(std::move(unsortedMap));
    }

};

TEST(CountingSorterTest, findMaxValueTest1) {
    uint16_t maxValue = 1024;
    var_map vm(static_cast<size_t>(maxValue + 1));
    for (uint16_t i = 0; i <= maxValue; ++i) {
        vm[i] = var(i, "");
    }

    int maxValueResult = CountingSorterTest::_findMaxValue(vm);

    ASSERT_EQ(maxValueResult, maxValue);
}

TEST(CountingSorterTest, findMaxValueTest2) {
    uint16_t maxValue = 1024;
    var_map vm(static_cast<size_t>(maxValue + 1));
    for (uint16_t i = 0; i <= maxValue; ++i) {
        vm[i] = var(0, "");
    }

    int maxValueResult = CountingSorterTest::_findMaxValue(vm);

    ASSERT_EQ(maxValueResult, 0);
}

TEST(CountingSorterTest, countValuesTest) {
    uint16_t mapSize  = 100;
    uint8_t seqSize = 5;

    var_map vm(static_cast<size_t>(mapSize));
    for (uint16_t i = 0; i < mapSize; ++i) {
        vm[i] = var(i / seqSize, "");
    }

    uint16_t maxValue = static_cast<uint16_t>((mapSize - 1) / seqSize);

    auto countArray = CountingSorterTest::_countValues(vm, maxValue);

    for (auto const& each : countArray) {
        EXPECT_EQ(each, 5);
    }
}

TEST(CountingSorterTest, sumUpTest) {
    size_t size = 100;
    std::vector<uint64_t> array(size);
    for (size_t i = 0; i < size; ++i) {
        array[i] = i;
    }

    auto res = CountingSorterTest::_sumUp(array);

    for (size_t i = 1; i < size; ++i) {
        EXPECT_EQ(res[i], res[i - 1] + array[i]);
    }
}

TEST(CountingSorterTest, postTest) {
    var_map unsortedArray = {
        var(0, "0"),
        var(7, "7"),
        var(5, "5"),
        var(0, "00"),
        var(4, "4"),
        var(1, "1"),
        var(7, "77"),
        var(1, "11")
    };
    std::vector<uint64_t> countArray = {
        2, 4, 4, 4, 5, 6, 6, 8
    };

    var_map expect_result = {
        var(0, "0"),
        var(0, "00"),
        var(1, "1"),
        var(1, "11"),
        var(4, "4"),
        var(5, "5"),
        var(7, "7"),
        var(7, "77")
    };

    auto happend_result = CountingSorterTest::_post(unsortedArray, countArray);

    ASSERT_EQ(happend_result, expect_result);
}

std::string getRandomString(size_t size) {
    static const char alphabet[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        tmp_s += alphabet[static_cast<size_t>(rand()) % (sizeof(alphabet) - 1)];
    }
    
    return tmp_s;
}

std::string getRandomCompletedString() {
    uint8_t str_size = static_cast<uint8_t>(rand() % (STRING_SIZE + 1)); 
    return getRandomString(str_size) + std::string(STRING_SIZE - str_size, ' ');
}

var_map generateRandomVarMap(size_t size) {
    var_map vm(size);
    for (size_t i = 0; i < size; ++i) {
        uint16_t j = static_cast<uint16_t>(rand() % UINT16_MAX);
        vm[i] = var(j, getRandomCompletedString());
    }
    return vm;
}

var_map stl_stable_sorted(var_map const& vm) {
    var_map expected_result = vm;
    std::stable_sort(expected_result.begin(), expected_result.end());
    return expected_result;
}

TEST(CountingSorterTest, sortGivenVarMapTest) {
    size_t size = 1000;
    auto vm = generateRandomVarMap(size);

    auto happened_result = CountingSorterTest::_sort(vm);

    auto expected_result = stl_stable_sorted(vm);

    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(happened_result[i], expected_result[i]);
    }
}


std::string generateRandomInput(size_t size) {
    std::string input;
    for (size_t i = 0; i < size; ++i) {
        uint16_t id = static_cast<uint16_t>(rand() % UINT16_MAX);
        std::string val = getRandomCompletedString();
        input += var(id, val).toString() + "\n";
    }
    return input;
}

TEST(MapReadingAndCountingSorterTest, sortGivenInputTest) {
    size_t size = 1000;
    auto input = generateRandomInput(size);
    std::istringstream iss(input);

    auto vm = readMap(iss);

    printMap(vm);
    std::cout << std::endl;

    auto happened_result = CountingSorterTest::_sort(vm);

    auto expected_result = stl_stable_sorted(vm);

    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(happened_result[i], expected_result[i]);
    }

    printMap(happened_result);
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}