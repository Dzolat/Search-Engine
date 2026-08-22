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

    double average_length = static_cast<double>(total_tokens) / total_documents;
    std::cout << "Indexed " << total_documents << " documents\n";
    std::cout << "Indexed " << total_tokens << " tokens\n";
    while (true)
    {
        auto search_info = start_search(doc_vector, index, average_length);
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

std::optional<std::string> load_text_file(const std::filesystem::path &path)
{
    const auto extension = path.extension().string();

    if (extension != ".txt" && extension != ".md")
        return std::nullopt;

    std::error_code error;
    const auto size = std::filesystem::file_size(path, error);

    if (error || size > Constants::MAX_FILE_SIZE)
        return std::nullopt;

    std::ifstream file(path, std::ios::binary);
    if (!file)
        return std::nullopt;

    std::string content{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    };

    if (content.find('\0') != std::string::npos)
        return std::nullopt;

    return content;
}

int get_documents(const std::filesystem::path &directory_path, std::vector<Document> &doc_vector)
{
    using namespace std::filesystem;
    int counter = 0;

    for (const auto &entry : recursive_directory_iterator(directory_path, directory_options::skip_permission_denied))
    {
        auto path = entry.path();
        auto file_content { load_text_file(path) };

        if (!file_content)
            continue;

        doc_vector.push_back(Document{counter, path.string(), *file_content});
        counter++;
    }
    return counter;
}