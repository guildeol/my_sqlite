#pragma once

#include <string>
#include <variant>

enum TokenType
{
    META_COMMAND_EXIT,

    KEYWORD_CREATE,
    KEYWORD_INSERT,
    KEYWORD_INTO,
    KEYWORD_FROM,
    KEYWORD_SELECT,
    KEYWORD_TABLE,
    KEYWORD_WHERE,

    LITERAL_STRING,
    LITERAL_NUMBER,
    LITERAL_FLOAT,

    IDENTIFIER,

    LEFT_PAREN,
    RIGHT_PAREN,
    COMMA,
    SEMICOLON,
    EQUAL,
    NOT_EQUAL,
};

using TokenValue = std::variant<std::nullptr_t, std::string, int, double>;

class Token
{
    public:
        TokenType type;
        TokenValue value;

        Token(TokenType p_type, TokenValue p_value = nullptr)
            : type(p_type), value(p_value)
        {
            // Empty
        }

        const char *to_string() const
        {
            return type_to_string(this->type);
        }

        static const char *type_to_string(TokenType p_type)
        {
            switch (p_type)
            {
                case META_COMMAND_EXIT: return "META_COMMAND_EXIT";
                case KEYWORD_CREATE: return "KEYWORD_CREATE";
                case KEYWORD_INSERT: return "KEYWORD_INSERT";
                case KEYWORD_INTO: return "KEYWORD_INTO";
                case KEYWORD_FROM: return "KEYWORD_FROM";
                case KEYWORD_SELECT: return "KEYWORD_SELECT";
                case KEYWORD_TABLE: return "KEYWORD_TABLE";
                case KEYWORD_WHERE: return "KEYWORD_WHERE";
                case LITERAL_STRING: return "LITERAL_STRING";
                case LITERAL_NUMBER: return "LITERAL_NUMBER";
                case LITERAL_FLOAT: return "LITERAL_FLOAT";
                case IDENTIFIER: return "IDENTIFIER";
                case LEFT_PAREN: return "LEFT_PAREN";
                case RIGHT_PAREN: return "RIGHT_PAREN";
                case COMMA: return "COMMA";
                case SEMICOLON: return "SEMICOLON";
                case EQUAL: return "EQUAL";
                case NOT_EQUAL: return "NOT_EQUAL";
            }

            return "UNKNOWN";
        }
};