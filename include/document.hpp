#pragma once

#include <string>
struct Document
{
    int id{0};
    std::string path{};
    std::string content{};
    int token_count{0};
};