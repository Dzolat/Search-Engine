#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "../include/document.hpp"
#include "../include/InvertedIndex.hpp"
#include "../include/Tokenizer.hpp"
#include "../include/Search.hpp"

constexpr unsigned long long operator""_gb(unsigned long long num)
{
    return num * 1'000'000'000ULL;
}

namespace Constants
{
    const int TOP_K {3};
    const int MAX_FILE_SIZE {1_gb};
}

int get_documents(const std::filesystem::path &directory_path, std::vector<Document> &doc_vector);
void print_results(std::vector<Document> &doc_vector, std::vector<std::pair<int, double>> &search_results);

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
    for (auto &doc : doc_vector)
    {
        int tokens = addDocument(doc, index);
        doc.token_count = tokens;
        total_tokens += tokens;
    }

    std::cout << "Indexed " << total_documents << " documents\n";
    std::cout << "Indexed " << total_tokens << " tokens\n";
    while (true)
    {
        auto search_info = start_search(doc_vector, index);
        if (!search_info)
            break;
        print_results(doc_vector, *search_info);
    }
}

void print_results(std::vector<Document> &doc_vector, std::vector<std::pair<int, double>> &search_info)
{
    if (search_info.size() == 0)
    {
        std::cout << "No results found";
        return;
    }
    for (int index = 0; index < std::min(Constants::TOP_K, static_cast<int>(search_info.size())); index++)
    {
        std::cout << "\n"
                  << index + 1 << ". " << doc_vector[search_info[index].first].path << "\n   score: " << search_info[index].second << "\n";
    }
    return;
}

int get_documents(const std::filesystem::path &directory_path, std::vector<Document> &doc_vector)
{
    using namespace std::filesystem;
    int counter = 0;

    auto validate_file = [](const auto &entry)
    {
        static std::vector<std::string> allowed_extensions{
            ".md",
            ".txt"};

        std::ifstream file(entry.path(), std::ios::binary);
        std::string extension = entry.path().extension().string();

        if (!file)
            return false;

        if (!entry.is_regular_file())
            return false;    

        if (std::find(allowed_extensions.begin(), allowed_extensions.end(), extension) == allowed_extensions.end())
            return false;

        if (file_size(entry) > Constants::MAX_FILE_SIZE)
            return false;

        char byte;
        while (file.get(byte))
        {
            if (byte == '\0')
                return false;
        }

        return true;
    };

    for (const auto &entry : recursive_directory_iterator(directory_path, directory_options::skip_permission_denied))
    {
        if (!validate_file(entry))
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