#pragma once

#include <string>
#include <map>
#include <vector>
#include <random>


#ifdef ALPHAORDER_DLL
#define EXPORTAPI __declspec(dllexport)
#else
#define EXPORTAPI __declspec(dllimport)
#endif

typedef std::map<char, int> alphabet_t;
typedef std::map<int, char> inversed_alphabet_t;

EXPORTAPI alphabet_t base_alphabet();
EXPORTAPI alphabet_t reversed_alphabet();
EXPORTAPI inversed_alphabet_t inverse_alphabet_map(alphabet_t const & alphabet);
EXPORTAPI std::string alphabet_string(alphabet_t const & alphabet);
EXPORTAPI bool is_ordered(std::string const & word, alphabet_t const & alphabet);
EXPORTAPI int alphabet_score(std::vector<std::string> const & words, alphabet_t const & alphabet);
EXPORTAPI void swap_letter(alphabet_t & alphabet, char const l1, char const l2);
EXPORTAPI void alter_alphabet(alphabet_t & alphabet, std::mt19937 & generator, std::uniform_int_distribution<int> & uniform_distribution);
EXPORTAPI void shuffle_alphabet(alphabet_t & alphabet);
EXPORTAPI std::vector<std::string> read_words();
EXPORTAPI std::pair<int, std::string> find_optimal_alphabet(std::vector<std::string> const & words, int const timeout);
EXPORTAPI std::pair<int, std::string> find_optimal_alphabet_async(std::vector<std::string> const & words, int const timeout, int const seed);

