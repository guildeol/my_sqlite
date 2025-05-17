#include <algorithm>
#include <unordered_map>
#include <optional>
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
    {"VALUES", KEYWORD_VALUES},
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
        else if (symbol == "*")
        {
            token_class = TokenType::WILDCARD;
        }
        else if (is_reserved(symbol))
        {
            token_class = reserved_words.at(symbol);
        }
        else if (is_integer(symbol))
        {
            token_class = TokenType::LITERAL_INTEGER;
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

static TokenType peek(std::vector<Token> &p_tokens)
{
    if (p_tokens.empty())
    {
        throw std::runtime_error("Unexpected end of input");
    }

    return p_tokens.front().type;
}

static void check_or(std::vector<Token> &p_tokens, TokenType p_token_type, const std::exception &p_exception)
{
    if (p_tokens.empty())
    {
        throw std::runtime_error("Unexpected end of input");
    }

    auto token = p_tokens.front();
    if (token.type != p_token_type)
    {
        throw p_exception;
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

static Token consume_or(std::vector<Token> &p_tokens, TokenType p_token_type, const std::exception &p_exception)
{
   check_or(p_tokens, p_token_type, p_exception);
   auto token = p_tokens.front();
   advance(p_tokens);

   return token;
}

template<typename T>
static std::vector<T> get_parenthesized_list(std::vector<Token> &p_tokens, TokenType p_expected_type)
{
    consume_or(p_tokens, TokenType::LEFT_PAREN, std::runtime_error("Expected '('"));

    auto list = std::vector<T>();
    while (!p_tokens.empty())
    {
        auto token = consume_or(p_tokens, p_expected_type, std::runtime_error("Expected type mismatch"));
        list.push_back(std::get<T>(token.value));

        if (p_tokens.front().type == TokenType::RIGHT_PAREN)
        {
            break;
        }

        consume_or(p_tokens, TokenType::COMMA, std::runtime_error("Expected ',' after list element"));
    }

    consume_or(p_tokens, TokenType::RIGHT_PAREN, std::runtime_error("Expected ')' after list"));

    return list;
}

static std::vector<Token> get_parenthesized_token_list(std::vector<Token> &p_tokens)
{
    consume_or(p_tokens, TokenType::LEFT_PAREN, std::runtime_error("Expected '('"));

    auto list = std::vector<Token>();
    while (!p_tokens.empty())
    {
        list.push_back(p_tokens.front());
        advance(p_tokens);

        if (p_tokens.front().type == TokenType::RIGHT_PAREN)
        {
            break;
        }

        consume_or(p_tokens, TokenType::COMMA, std::runtime_error("Expected ',' after list element"));
    }

    consume_or(p_tokens, TokenType::RIGHT_PAREN, std::runtime_error("Expected ')' after list"));

    return list;
}

static void handle_create(std::vector<Token> &p_tokens)
{
    consume_or(p_tokens, TokenType::KEYWORD_TABLE, std::runtime_error("Expected 'TABLE' keyword"));

    auto table_name = consume_or(p_tokens, TokenType::IDENTIFIER, std::runtime_error("Expected table name"));
    auto columns = get_parenthesized_list<std::string>(p_tokens, TokenType::IDENTIFIER);

    TableManager::add_table(std::get<std::string>(table_name.value), columns);
}

static void handle_insert(std::vector<Token> &p_tokens)
{
    consume_or(p_tokens, TokenType::KEYWORD_INTO, std::runtime_error("Expected 'INTO' keyword"));

    auto table_name = consume_or(p_tokens, TokenType::IDENTIFIER, std::runtime_error("Expected table name"));

    auto columns = get_parenthesized_list<std::string>(p_tokens, TokenType::IDENTIFIER);

    consume_or(p_tokens, TokenType::KEYWORD_VALUES, std::runtime_error("Expected 'VALUES' keyword"));

    auto rows = get_parenthesized_token_list(p_tokens);

    auto &table = TableManager::find(std::get<std::string>(table_name.value));
    table.insert(columns, rows);
}

static void handle_select(std::vector<Token> &p_tokens)
{
    auto next_token = peek(p_tokens);
    if (next_token == TokenType::WILDCARD)
    {
        consume_or(p_tokens, TokenType::WILDCARD, std::runtime_error("Expected '*' wildcard"));
    }
    else if (next_token == TokenType::LEFT_PAREN)
    {
        auto columns = get_parenthesized_list<std::string>(p_tokens, TokenType::IDENTIFIER);
    }
    else
    {
        throw std::runtime_error("Expected '*' or  parenthesized list after table name");
    }

    consume_or(p_tokens, TokenType::KEYWORD_FROM, std::runtime_error("Expected 'FROM' keyword"));

    auto table_name = consume_or(p_tokens, TokenType::IDENTIFIER, std::runtime_error("Expected table name"));

    auto table = TableManager::find(std::get<std::string>(table_name.value));
    table.select_all();
}

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
            break;

            case KEYWORD_CREATE:
                handle_create(p_tokens);
            break;

            case KEYWORD_INSERT:
                handle_insert(p_tokens);
            break;

            case KEYWORD_SELECT:
               handle_select(p_tokens);
            break;

            case LITERAL_STRING:
                // Handle string literal
            break;

            case LITERAL_INTEGER:
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