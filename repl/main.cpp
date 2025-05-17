#include <iostream>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "input_buffer.h"
#include "meta_command.h"
#include "parser.h"

static bool use_prompt = true;

static void print_prompt()
{
    fmt::print("db > ");
}

static void process_arguments(int argc, char *argv[])
{
   for (auto i = 1; i < argc; i++)
    {
         if (std::string(argv[i]) == "--no-prompt")
         {
              use_prompt = false;
         }
    }
}

int main(int argc, char *argv[])
{
    process_arguments(argc, argv);

    auto input_buffer = InputBuffer();

    while (true)
    {
        if (use_prompt)
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