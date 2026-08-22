#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

#include "../include/Tokenizer.hpp"

std::vector<std::string> tokenize(const std::string& text)
{
    std::string cleaned;

    for (int index = 0, length = text.size(); index < length; index++)
    {
        unsigned char character = static_cast<unsigned char>(text[index]);
        if (std::isalnum(character))
            cleaned += std::tolower(character);
        else
            cleaned += ' ';
    }

    std::stringstream ss(cleaned);
    std::vector<std::string> tokens;
    
    std::string word;
    while (ss >> word)
        tokens.push_back(word);

    return tokens;
}