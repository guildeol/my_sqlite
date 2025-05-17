cmake_minimum_required(VERSION 3.20)

# Project name and version
project(my_sqlite VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/releases/download/11.2.0/fmt-11.2.0.zip
)
FetchContent_MakeAvailable(fmt)

# Add source files
file(GLOB_RECURSE SOURCES "lib/*.cpp")

# Add the library
add_library(sqlite STATIC ${SOURCES})

#Include directories
target_include_directories(sqlite PUBLIC "lib")
target_link_libraries(sqlite PUBLIC fmt::fmt)

add_subdirectory(tests)

add_executable(my_sqlite "repl/main.cpp")
target_link_libraries(my_sqlite PRIVATE sqlite)

target_compile_options(sqlite PRIVATE -Wall -Wextra -Wpedantic -Werror -g3)
target_compile_options(my_sqlite PRIVATE -Wall -Wextra -Wpedantic -Werror -g3)
