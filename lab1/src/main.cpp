#include <lib.hpp>
#include <sstream>

using namespace da_lab1;

int main() {
    auto unsorted_map = readMap(std::cin);

    auto sorted_map = CountingSorter::sort(std::move(unsorted_map));

    printMap(sorted_map);

    return 0;
}