#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "../include/TF-IDF.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/document.hpp"

std::vector<std::pair<int, double>> start_search(std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index)
{
    std::string phrase;

    std::cout << "\nsearch> ";

    if (!std::getline(std::cin >> std::ws, phrase))
        return std::vector<std::pair<int, double>> {};

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
            double tf = get_tf(frequency, doc_vector[document_id].token_count);
            double idf = get_idf(query_word, doc_vector, index);
            double ranking = tf * idf;
            scores[document_id] += ranking;
        }
    }

    std::vector<std::pair<int, double>> results(scores.begin(), scores.end());
    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });
    return results;
}