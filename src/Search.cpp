#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "../include/Tokenizer.hpp"
#include "../include/document.hpp"

void start_search(std::vector<Document>& doc_vector, std::unordered_map<std::string, std::unordered_map<int, int>> &index)
{
    std::string phrase;

    std::cout << "\nsearch> ";
    std::getline(std::cin >> std::ws, phrase);

    std::unordered_map<int, int> scores;
    for (const auto &query_word : tokenize(phrase))
    {

        auto it = index.find(query_word);

        if (it == index.end())
        {
            continue;
        }

        for (const auto &[document_id, frequency] : it->second)
        {
            scores[document_id] += frequency;
        }
    }

    std::vector<std::pair<int, double>> results(scores.begin(), scores.end());
    std::sort(results.begin(), results.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    int counter = 1;
    for (auto result : results)
    {
        if (counter == 3)
            break;
        std::cout << "\n" << counter << ". " << doc_vector[result.first].path << "\n   score: " << result.second << "\n";
        counter++;
        
    }
}