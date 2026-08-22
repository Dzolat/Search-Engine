#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "../include/document.hpp"
#include "../include/Tokenizer.hpp"

double get_tf(double frequency, double token_count)
{
    return frequency / token_count;
}

double get_idf(std::string term, std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index)
{
    auto indexed_term = index.find(term);
    if (indexed_term == index.end())
        return 0.0;

    const double document_count = static_cast<double>(doc_vector.size());
    const double document_frequency = static_cast<double>(indexed_term->second.size());
    return std::log(document_count / document_frequency);
}