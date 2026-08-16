#pragma once

#include <string>
#include <unordered_map>

#include "Tokenizer.hpp"
#include "document.hpp"

bool start_search(std::vector<Document>& doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index);