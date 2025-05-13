# Enable testing
enable_testing()

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

# Glob for all test source files in the suites directory
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/suites/*.cpp")

# Add test executable
add_executable(test_sqlite ${TEST_SOURCES})

# Link GoogleTest libraries
target_link_libraries(test_sqlite PRIVATE gtest_main libsqlite)
target_compile_options(test_sqlite PRIVATE -Wall -Wextra -Wpedantic -Werror -g3)