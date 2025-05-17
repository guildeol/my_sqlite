#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "token.h"

using ColumnsEntry_t = std::vector<std::string>;
using RowEntry_t = std::vector<Token>;

class Table
{
    public:
        std::string name;
        std::vector<std::string> columns;
        std::vector<RowEntry_t> rows;

        Table(std::string p_name, std::vector<std::string> p_columns);

        void insert(const ColumnsEntry_t &p_columns, const RowEntry_t &p_row);
        void select_all();
};

class TableManager
{
    public:
        static void add_table(std::string p_name, std::vector<std::string> p_columns);
        static Table &find(std::string p_name);

    private:
        inline static std::map<std::string, Table> table_map;
};