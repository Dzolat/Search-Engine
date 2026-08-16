#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "../include/TF-IDF.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/document.hpp"

bool start_search(std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index)
{
    std::string phrase;

    std::cout << "\nsearch> ";

    if (!std::getline(std::cin >> std::ws, phrase))
        return false;

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
            double tf_idf = get_tf(query_word, doc_vector[document_id]) * get_idf(query_word, doc_vector, index);
            scores[document_id] += tf_idf;
        }
    }

    std::vector<std::pair<int, double>> results(scores.begin(), scores.end());
    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    int counter = 0;
    for (auto result : results)
    {
        if (counter == 3)
            break;
        std::cout << "\n"
                  << counter++ << ". " << doc_vector[result.first].path << "\n   score: " << result.second << "\n";
    }

    return true;
}