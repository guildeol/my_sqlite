#include <cstdlib>
#include <stdexcept>

#include <fmt/core.h>

#include "token.h"
#include "meta_command.h"

void MetaCommand::execute(const TokenType p_input)
{
    if (p_input == META_COMMAND_EXIT)
    {
        std::exit(0);
    }

    throw std::invalid_argument(fmt::format("Unrecognized meta command '{}'", Token::type_to_string(p_input)));
}