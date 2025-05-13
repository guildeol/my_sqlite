#pragma once

#include <string>
#include <vector>

class InputBuffer
{
    public:
        static std::vector<std::string> read(std::istream &p_stream);
};