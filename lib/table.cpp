#include <stdexcept>

#include "table.h"

Table::Table(std::string p_name, std::vector<std::string> p_columns)
    : name(p_name), columns(p_columns)
{
    // Empty
}

void Table::insert(RowEntry_t p_row)
{
    if (p_row.size() != columns.size())
    {
        throw std::invalid_argument("Row size does not match table columns");
    }

    rows.push_back(p_row);
}
