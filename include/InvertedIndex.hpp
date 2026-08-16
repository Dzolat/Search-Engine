#include <unordered_map>

#include "document.hpp"
#include "Tokenizer.hpp"

int addDocument(const Document &doc, std::unordered_map<std::string, std::unordered_map<int, double>> &indexes);