#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "../include/document.hpp"
#include "../include/Tokenizer.hpp"

double get_tf(std::string term, const Document &doc)
{
    double totalTerm = 0;
    auto tokens = tokenize(doc.content);
    for (const auto &s : tokens)
    {
        if (s == term)
            totalTerm++;
    }
    if (tokens.size() == 0)
        return 0.0;
    return totalTerm / tokens.size();
}

double get_idf(std::string term, std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index)
{
    auto indexed_term = index.find(term);
    if (indexed_term == index.end())
        return 0.0;
    return std::log(doc_vector.size() / indexed_term->second.size());
}