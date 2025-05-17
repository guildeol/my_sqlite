#include <stdexcept>
#include <map>

#include <fmt/core.h>

#include "table.h"

Table::Table(std::string p_name, std::vector<std::string> p_columns)
    : name(p_name), columns(p_columns)
{
    // Empty
}

void Table::insert(const ColumnsEntry_t &p_columns, const RowEntry_t &p_row)
{
    if (p_row.size() != p_columns.size())
    {
        throw std::invalid_argument("Row size does not match table columns");
    }

    this->rows.push_back(p_row);
}

void Table::select_all()
{
    for (const auto &row : this->rows)
    {
        for (size_t i = 0; i < columns.size(); ++i)
        {
            fmt::print("{}", row[i].print_value());

            if (i < columns.size() - 1)
            {
                fmt::print("|");
            }
        }

        fmt::print("\n");
    }
}

void TableManager::print_tables()
{
    for (const auto &[name, _] : table_map)
    {
        fmt::print("{}\n", name);
    }
}

void TableManager::add_table(std::string p_name, std::vector<std::string> p_columns)
{
    if (table_map.find(p_name) != table_map.end())
    {
        throw std::invalid_argument("Table already exists");
    }

    table_map.try_emplace(p_name, Table(p_name, p_columns));
}

Table &TableManager::find(std::string p_name)
{
    if (!table_map.contains(p_name))
    {
        throw std::invalid_argument(fmt::format("Table '{}' not found", p_name));
    }

    return table_map.at(p_name);
}
