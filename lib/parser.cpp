#include <algorithm>
#include <unordered_map>
#include <stdexcept>

#include <fmt/core.h>

#include "meta_command.h"
#include "parser.h"
#include "table.h"

static const std::unordered_map<std::string, TokenType> reserved_words = {
    {".exit", META_COMMAND_EXIT},
    {"CREATE", KEYWORD_CREATE},
    {"INSERT", KEYWORD_INSERT},
    {"INTO", KEYWORD_INTO},
    {"FROM", KEYWORD_FROM},
    {"SELECT", KEYWORD_SELECT},
    {"TABLE", KEYWORD_TABLE},
    {"WHERE", KEYWORD_WHERE},
};

bool is_reserved(const std::string &p_symbol)
{
    return reserved_words.find(p_symbol) != reserved_words.end();
}

bool is_integer(const std::string &p_symbol)
{
    if (p_symbol.empty())
    {
        return false;
    }

    return std::all_of(p_symbol.begin(), p_symbol.end(), [](char c){return std::isdigit(c);});
}

bool is_float(const std::string &p_symbol)
{
    if (p_symbol.empty())
    {
        return false;
    }

    bool decimal_found = false;

    for (const auto &c : p_symbol)
    {
        if (c == '.')
        {
            if (decimal_found)
            {
                return false; // More than one decimal point
            }
            decimal_found = true;
        }
        else if (!std::isdigit(c))
        {
            return false; // Non-digit character found
        }
    }

    return true;
}

bool is_string(const std::string &p_symbol)
{
    if (p_symbol.empty())
    {
        return false;
    }

    // Check if the string is enclosed in double quotes
    return p_symbol.front() == '"' && p_symbol.back() == '"';
}

std::vector<Token> Parser::tokenize(const std::vector<std::string> &p_input)
{
    std::vector<Token> tokens;
    tokens.reserve(p_input.size());

    if (p_input.empty())
    {
        throw std::invalid_argument("Input is empty");
    }

    for (const auto &symbol : p_input)
    {
        TokenType token_class;
        TokenValue token_value = nullptr;

        if (symbol.empty())
        {
            throw std::runtime_error("Empty symbol found: parsing error");
        }

        if (symbol == "(")
        {
            token_class = TokenType::LEFT_PAREN;
        }
        else if (symbol == ")")
        {
            token_class = TokenType::RIGHT_PAREN;
        }
        else if (symbol == ",")
        {
            token_class = TokenType::COMMA;
        }
        else if (symbol == ";")
        {
            token_class = TokenType::SEMICOLON;
        }
        else if (symbol == "=")
        {
            token_class = TokenType::EQUAL;
        }
        else if (symbol == "!=")
        {
            token_class = TokenType::NOT_EQUAL;
        }
        else if (is_reserved(symbol))
        {
            token_class = reserved_words.at(symbol);
        }
        else if (is_integer(symbol))
        {
            token_class = TokenType::LITERAL_NUMBER;
            token_value = std::stoi(symbol);
        }
        else if (is_float(symbol))
        {
            token_class = TokenType::LITERAL_FLOAT;
            token_value = std::stod(symbol);
        }
        else if (is_string(symbol))
        {
            token_class = TokenType::LITERAL_STRING;
            token_value = symbol;
        }
        else
        {
            token_class = TokenType::IDENTIFIER;
            token_value = symbol;
        }

        tokens.emplace_back(token_class, token_value);
    }

    return tokens;
}

static void check_or(std::vector<Token> &p_tokens, TokenType p_token_type, std::string p_error_msg)
{
    if (p_tokens.empty())
    {
        throw std::runtime_error("Unexpected end of input");
    }

    auto token = p_tokens.front();
    if (token.type != p_token_type)
    {
        throw std::runtime_error(p_error_msg);
    }
}

static void advance(std::vector<Token> &p_tokens)
{
    if (p_tokens.empty())
    {
        throw std::runtime_error("Unexpected end of input");
    }

    p_tokens.erase(p_tokens.begin());
}

static void handle_create(std::vector<Token> &p_tokens)
{
    check_or(p_tokens, TokenType::KEYWORD_TABLE, "Expected 'TABLE' keyword");
    advance(p_tokens);

    check_or(p_tokens, TokenType::IDENTIFIER, "Expected table name");
    auto table_name = p_tokens.front().value;
    advance(p_tokens);

    check_or(p_tokens, TokenType::LEFT_PAREN, "Expected '(' after table name");
    advance(p_tokens);

    auto columns = std::vector<std::string>();
    while (!p_tokens.empty())
    {

        check_or(p_tokens, TokenType::IDENTIFIER, "Expected column name");
        columns.push_back(std::get<std::string>(p_tokens.front().value));
        advance(p_tokens);


        if (p_tokens.front().type == TokenType::RIGHT_PAREN)
        {
            break;
        }

        check_or(p_tokens, TokenType::COMMA, "Expected ',' after column name");
        advance(p_tokens);
    }

    auto table = new Table(std::get<std::string>(table_name), columns);

    check_or(p_tokens, TokenType::RIGHT_PAREN, "Expected ')' after column definitions");
    advance(p_tokens);
}

// static void handle_select(std::vector<Token> &p_tokens)
// {
//     if (check(p_tokens, TokenType::IDENTIFIER))
//     {
//         auto what = p_tokens.front().value;
//     }
// }

void Parser::evaluate(std::vector<Token> &p_tokens)
{
    while(!p_tokens.empty())
    {
        auto token = p_tokens.front();
        advance(p_tokens);

        switch (token.type)
        {
            case META_COMMAND_EXIT:
                MetaCommand::execute(token.type);

                fmt::print("Executed meta command: '{}'\n", token.to_string());
            break;

            case KEYWORD_CREATE:
                handle_create(p_tokens);
                fmt::print("Executed create command: '{}'\n", token.to_string());
            break;

            case KEYWORD_INSERT:
                // Handle INSERT statement
            break;

            case KEYWORD_SELECT:
                // Handle SELECT statement
            break;

            case LITERAL_STRING:
                // Handle string literal
            break;

            case LITERAL_NUMBER:
                // Handle number literal
            break;

            case LITERAL_FLOAT:
                // Handle float literal
            break;

            default:
                throw std::invalid_argument(fmt::format("Unrecognized token type '{}'", token.to_string()));
            return;
        }
    }
}