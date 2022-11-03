#pragma once
#include "sqlite3.h"

namespace db {
    sqlite3* db;

    std::string Append_Quotes (std::string &string) {
        return "'" + string + "'";
    }
}