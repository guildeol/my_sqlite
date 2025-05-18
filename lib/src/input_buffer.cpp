#include <iostream>

#include "input_buffer.h"

static std::vector<std::string> split(std::string &p_input)
{
    std::vector<std::string> symbols;
    std::string symbol;
    bool in_quotes = false;

    for (const auto c: p_input)
    {
        if (c == '(' || c == ')' || c == ',' || c == ';' || c == '=' || c == '!')
        {
            if (!symbol.empty())
            {
                symbols.push_back(symbol);
                symbol.clear();
            }

            symbols.push_back(std::string(sizeof(char), c));

            continue;
        }

        if (!std::isspace(c) || in_quotes)
        {
            if (c == '"')
            {
                in_quotes = !in_quotes;
            }

            symbol.push_back(c);
        }
        else if (!symbol.empty())
        {
            symbols.push_back(symbol);
            symbol.clear();
        }
    }

    // Push the last symbol if it exists
    if (!symbol.empty())
    {
        symbols.push_back(symbol);
    }

    return symbols;
}

std::vector<std::string> InputBuffer::read(std::istream &p_stream)
{
    std::string input;

    std::getline(p_stream, input);

    return split(input);
}
