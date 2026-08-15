#include <unordered_map>

#include "../include/document.hpp"
#include "../include/Tokenizer.hpp"

int addDocument(const Document &doc, std::unordered_map<std::string, std::unordered_map<int, int>> &indexes)
{
    auto tokens = tokenize(doc.content);

    for (auto token : tokens)
    {
        indexes[token][doc.id]++;
    }

    return tokens.size();
}