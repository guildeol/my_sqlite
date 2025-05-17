#include <cstdlib>
#include <stdexcept>

#include <fmt/core.h>

#include "table.h"
#include "token.h"
#include "meta_command.h"

void MetaCommand::execute(const TokenType p_input)
{
    switch (p_input)
    {
        case META_COMMAND_EXIT:
            std::exit(0);
        break;

        case META_COMMAND_TABLES:
            TableManager::print_tables();
        break;

        default:
            throw std::invalid_argument(fmt::format("Unrecognized meta command '{}'", Token::type_to_string(p_input)));
        break;
    }
}