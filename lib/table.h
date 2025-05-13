#pragma once

#include <string>
#include <vector>

#include "token.h"

using RowEntry_t = std::vector<TokenValue>;

class Table
{
    public:
        std::string name;
        std::vector<std::string> columns;
        std::vector<RowEntry_t> rows;

        Table(std::string p_name, std::vector<std::string> p_columns);

        void insert(RowEntry_t p_row);
};