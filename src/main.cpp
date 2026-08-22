#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <string>
#include <unordered_map>

#include "../include/document.hpp"
#include "../include/InvertedIndex.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/Search.hpp"

int get_documents(const std::filesystem::path &directory_path, std::vector<Document> &doc_vector);

int main(int argc, char *argv[])
{
    std::vector<Document> doc_vector;
    std::unordered_map<std::string, std::unordered_map<int, double>> index;

    if (argc != 2)
    {
        std::cout << "Usage: ./search.exe [path]";
        return 1;
    }

    std::filesystem::path directory_path = argv[1];
    if (!exists(directory_path) or !std::filesystem::is_directory(directory_path))
    {
        std::cout << "The directory doesn't exist.";
        return 2;
    }

    std::cout << "Indexing documents...\n\n";

    int total_documents = get_documents(directory_path, doc_vector);
    int total_tokens = 0;
    for (const auto &doc : doc_vector)
    {
        int tokens = addDocument(doc, index);
        total_tokens += tokens;
    }

    std::cout << "Indexed " << total_documents << " documents\n";
    std::cout << "Indexed " << total_tokens << " tokens\n";
    while (true)
    {
        auto search_info = start_search(doc_vector, index);
        for (int index = 0; index < 3; index++)
        {
            int counter = 0;

            std::cout << "\n" << index << ". " << doc_vector[search_info[index].first].path << "\n   score: " << search_info[index].second << "\n";
        }
    }
}

int get_documents(const std::filesystem::path &directory_path, std::vector<Document> &doc_vector)
{
    using namespace std::filesystem;
    int counter = 0;
    for (const auto &entry : recursive_directory_iterator(directory_path))
    {
        if (!entry.is_regular_file())
        {
            continue;
        }

        auto path = entry.path();

        std::ifstream file{path};

        if (!file)
        {
            continue;
        }

        std::string file_content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

        doc_vector.push_back(Document{counter, path.string(), file_content});
        counter++;
    }
    return counter;
}