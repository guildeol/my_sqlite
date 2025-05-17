#include <gtest/gtest.h>
#include <fmt/core.h>

#include "parser.h"

TEST(ParserTest, ShouldTokenizeEmptyInput)
{
    EXPECT_THROW(Parser::tokenize({}), std::invalid_argument);
}

TEST(ParserTest, ShouldTokenizeMetaCommand)
{
    auto tokens = Parser::tokenize({".exit"});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, META_COMMAND_EXIT);
    EXPECT_EQ(std::get<std::nullptr_t>(tokens[0].value), nullptr);
}

TEST(ParserTest, ShouldTokenizeKeyword)
{
    auto tokens = Parser::tokenize({"SELECT"});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, KEYWORD_SELECT);
    EXPECT_EQ(std::get<std::nullptr_t>(tokens[0].value), nullptr);
}

TEST(ParserTest, ShouldTokenizeStringLiteral)
{
    auto tokens = Parser::tokenize({"\"Hello, World!\""});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, LITERAL_STRING);
    EXPECT_EQ(std::get<std::string>(tokens[0].value), "\"Hello, World!\"");
}

TEST(ParserTest, ShouldTokenizeIntegerLiteral)
{
    auto tokens = Parser::tokenize({"12345"});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, LITERAL_INTEGER);
    EXPECT_EQ(std::get<int>(tokens[0].value), 12345);
}

TEST(ParserTest, ShouldTokenizeFloatLiteral)
{
    auto tokens = Parser::tokenize({"123.45"});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, LITERAL_FLOAT);
    EXPECT_EQ(std::get<double>(tokens[0].value), 123.45);
}

TEST(ParserTest, ShouldTokenizeIdentifier)
{
    auto tokens = Parser::tokenize({"my_variable"});

    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[0].value), "my_variable");
}
TEST(ParserTest, ShouldTokenizeComplexInput)
{
    auto tokens = Parser::tokenize({"SELECT", "table", "FROM", "users", "WHERE", "name", "=", "\"John Doe\""});

    EXPECT_EQ(tokens.size(), 8);
    EXPECT_EQ(tokens[0].type, KEYWORD_SELECT);
    EXPECT_EQ(tokens[1].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[1].value), "table");
    EXPECT_EQ(tokens[2].type, KEYWORD_FROM);
    EXPECT_EQ(tokens[3].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[3].value), "users");
    EXPECT_EQ(tokens[4].type, KEYWORD_WHERE);
    EXPECT_EQ(tokens[5].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[5].value), "name");
    EXPECT_EQ(tokens[6].type, EQUAL);
    EXPECT_EQ(tokens[7].type, LITERAL_STRING);
    EXPECT_EQ(std::get<std::string>(tokens[7].value), "\"John Doe\"");
}

TEST(ParserTest, ShouldTokenizeParentheses)
{
    auto tokens = Parser::tokenize({"(", "SELECT", "table", "FROM", "users", ")"});

    EXPECT_EQ(tokens.size(), 6);
    EXPECT_EQ(tokens[0].type, LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, KEYWORD_SELECT);
    EXPECT_EQ(tokens[2].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[2].value), "table");
    EXPECT_EQ(tokens[3].type, KEYWORD_FROM);
    EXPECT_EQ(tokens[4].type, IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(tokens[4].value), "users");
    EXPECT_EQ(tokens[5].type, RIGHT_PAREN);
}