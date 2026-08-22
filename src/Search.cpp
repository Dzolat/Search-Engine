#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <optional>

#include "../include/BM25.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/document.hpp"

std::optional<std::vector<std::pair<int, double>>> start_search(std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index, double average_document_length)
{
    std::string phrase;

    std::cout << "\nsearch> ";

    if (!std::getline(std::cin >> std::ws, phrase))
        return std::nullopt;

    std::unordered_map<int, double> scores;
    for (const auto &query_word : tokenize(phrase))
    {

        auto it = index.find(query_word);

        if (it == index.end())
        {
            continue;
        }

        for (const auto &[document_id, frequency] : it->second)
        {
            double document_length{static_cast<double>(doc_vector[document_id].token_count)};
            double document_count{static_cast<double>(doc_vector.size())};
            double document_frequency{static_cast<double>(it->second.size())};
            double ranking{bm25(frequency, document_length, average_document_length, document_count, document_frequency)};
            scores[document_id] += ranking;
        }
    }

    std::vector<std::pair<int, double>> results(scores.begin(), scores.end());
    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });
    return results;
}