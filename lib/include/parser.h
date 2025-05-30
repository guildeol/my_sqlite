#pragma once

#include <string>
#include <vector>

#include "token.h"

class Parser
{
    public:
        static std::vector<Token> tokenize(const std::vector<std::string> &p_input);
        static void evaluate(std::vector<Token> &p_tokens);
};
