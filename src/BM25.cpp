#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "../include/document.hpp"
#include "../include/Tokenizer.hpp"

double bm25(double frequency, double document_length, double average_document_length, double document_count, double document_frequency)
{
    double k1{1.2}, b{0.75};

    double idf{std::log((document_count - document_frequency + 0.5) / (document_frequency + 0.5) + 1) };
    double dividend{frequency * (k1 + 1)};
    double divisor{frequency + k1 * (1 - b + b * (document_length / average_document_length))};

    return idf * (dividend / divisor);
}