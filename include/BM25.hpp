#pragma once

#include "document.hpp"

double bm25(double frequency, double document_length, double average_document_length, double document_count, double document_frequency);
