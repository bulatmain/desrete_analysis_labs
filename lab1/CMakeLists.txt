cmake_minimum_required(VERSION 3.14)

project(da_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DA_LAB1_TESTING "Включить модульное тестирование" ON)

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -g
        # -Wall
        # -Wextra
        # -Wpedantic

        # -Wcast-align
        # -Wcast-qual
        # -Wconversion
        # -Wctor-dtor-privacy
        # -Wenum-compare
        # -Wfloat-equal
        # -Wnon-virtual-dtor
        # -Wold-style-cast
        # -Woverloaded-virtual
        # -Wredundant-decls
        # -Wsign-conversion
        # -Wsign-promo
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX)
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


# Lib include/
add_library(da_lab1_headers INTERFACE)
target_include_directories(da_lab1_headers INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(da_lab1_headers INTERFACE cxx_std_20)
add_library(da_lab1::headers ALIAS da_lab1_headers)

# Executable main
add_executable(da_lab1_main src/main.cpp)
target_link_libraries(da_lab1_main PUBLIC da_lab1::headers)
target_compile_features(da_lab1_main PUBLIC cxx_std_20)

# # Tests
# if(NOT DA_LAB1_TESTING)
#     message(STATUS "Тестирование выключено")
# else()
#     add_subdirectory(test)
# endif()

