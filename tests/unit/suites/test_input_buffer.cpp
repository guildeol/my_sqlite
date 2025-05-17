#include <gtest/gtest.h>
#include <fmt/core.h>

#include "input_buffer.h"

TEST(InputBufferTest, ShouldReadEmptyInput)
{
    auto empty_input = std::istringstream("");
    auto symbols = InputBuffer::read(empty_input);

    EXPECT_TRUE(symbols.empty());
}

TEST(InputBufferTest, ShouldReadValidInput)
{
    // Simulate user input
    auto input = std::istringstream("SELECT * FROM users");
    auto symbols = InputBuffer::read(input);

    auto expected_symbols = std::vector<std::string>{"SELECT", "*", "FROM", "users"};
    EXPECT_EQ(symbols,  expected_symbols);
}

TEST(InputBufferTest, ShouldReadQuotedString)
{
    // Simulate user input
    auto input = std::istringstream("SELECT * FROM users WHERE name = \"John Doe\"");
    auto symbols = InputBuffer::read(input);

    auto expected_symbols = std::vector<std::string>{"SELECT", "*", "FROM", "users", "WHERE", "name", "=", "\"John Doe\""};
    EXPECT_EQ(symbols,  expected_symbols);
}

TEST(InputBufferTest, ShouldReadComplexInput)
{
    // Simulate user input
    auto input = std::istringstream("CREATE TABLE users (id INTEGER, name TEXT, age INTEGER)");
    auto symbols = InputBuffer::read(input);

    auto expected_symbols = std::vector<std::string>{"CREATE", "TABLE", "users", "(", "id", "INTEGER", ",", "name", "TEXT", ",", "age", "INTEGER", ")"};
    EXPECT_EQ(symbols,  expected_symbols);
}
