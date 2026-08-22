cmake_minimum_required(VERSION 3.23)

project(SearchEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(search
    src/main.cpp
    src/Tokenizer.cpp
    src/InvertedIndex.cpp
    src/Search.cpp
    src/TF-IDF.cpp
)