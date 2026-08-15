#include <iostream>
#include <fstream>
#include <filesystem>

#include "../include/document.hpp"

int main(int argc, char *argv[])
{
    using namespace std::filesystem;
    std::vector<Document> doc_vector;

    if (argc != 2)
    {
        std::cout << "Usage: ./search.exe [path]";
        return 1;
    }

    path directory_path = argv[1];

    if (!exists(directory_path))
    {
        std::cout << "The directory doesnt exist.";
        return 2;
    }
}