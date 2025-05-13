#include <iostream>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "input_buffer.h"
#include "meta_command.h"
#include "parser.h"

static void print_prompt()
{
    fmt::print("db > ");
}

int main()
{
    auto input_buffer = InputBuffer();

    while (true)
    {
        print_prompt();

        try
        {
            auto symbols = input_buffer.read(std::cin);

            auto tokens = Parser::tokenize(symbols);

            Parser::evaluate(tokens);
        }

        catch (const std::exception &e)
        {
            fmt::print("Error: {}\n", e.what());
            continue;
        }
    }

    return 0;
}