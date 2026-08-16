#include <string>
#include <vector>
#include <unordered_map>

#include "document.hpp"

double get_tf(std::string term, const Document &doc);

double get_idf(std::string term, std::vector<Document> &doc_vector, std::unordered_map<std::string, std::unordered_map<int, double>> &index);