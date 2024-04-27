#include <gtest/gtest.h>

#include <array.hpp>

template <class T>
class TestArray : public Array<T> {
public:
    size_t& capacity() {
        return _capacity;
    }

    size_t& size() {
        return _size;
    }

    std::shared_ptr<T[]> array() {
        return array;
    }
};

TEST(ArrayTest, DefaultConstructorTest) {
    TestArray<int> arr;

    EXPECT_EQ(arr.capacity(), 0);
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.MULTIPLIER, 2);
    ASSERT_FALSE(static_cast<bool>(arr.array()));
}

TEST(ArrayTest, SIZE_T_ConstructorTest) {
    TestArray<int> arr(100u);

    EXPECT_EQ(arr.capacity(), 100);
    EXPECT_EQ(arr.size(), 100);
    ASSERT_TRUE(static_cast<bool>(arr.array()));
}

TEST(ArrayTest, SIZE_T_and_T_CONST_REF_ConstructorTest) {
    TestArray<int> arr(100u, 5);

    EXPECT_EQ(arr.capacity(), 100);
    EXPECT_EQ(arr.size(), 100);
    EXPECT_TRUE(static_cast<bool>(arr.array()));
    for (size_t i = 0; i < 100u; ++i) {
        EXPECT_EQ(arr[i], 5);
    }
}



int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  
  return RUN_ALL_TESTS();
}