#include "OptimalAlphaOrderCore.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <future>
#include <assert.h>
#include <filesystem>

alphabet_t base_alphabet() {
	alphabet_t alpha;

	for (int i = 0; i < 26; ++i)
		alpha.insert(std::make_pair(static_cast<char>('A' + i), i));

	return alpha;
}

alphabet_t reversed_alphabet() {
	alphabet_t alpha;

	for (int i = 25; i >= 0; i--)
		alpha.insert(std::make_pair(static_cast<char>('Z' - i), i));

	return alpha;
}

inversed_alphabet_t inverse_alphabet_map(alphabet_t const& alphabet) {
	inversed_alphabet_t inversedalpha;

	for (auto const& elem : alphabet)
		inversedalpha.insert(std::make_pair(elem.second, elem.first));

	return inversedalpha;
}

std::string alphabet_string(alphabet_t const& alphabet) {
	// initialize a string of alphabet.size() space (' ') elements
	std::string alphabet_string(alphabet.size(), ' ');

	auto reversed = inverse_alphabet_map(alphabet);

	std::transform(
		reversed.begin(), reversed.end(),
		alphabet_string.begin(),
		[](std::pair<int, char> const& elem) { return elem.second; }
	);

	return alphabet_string;
}

void swap_letter(alphabet_t &alphabet, char const l1, char const l2) {
	std::swap(alphabet[l1], alphabet[l2]);
}

bool is_ordered(std::string const& word, alphabet_t const& alphabet)
{
	if (word.size() < 2)
		return true;

	for (size_t i = 1; i < word.size(); ++i) {
		auto lpos1 = alphabet.find(word[i]);
		auto lpos2 = alphabet.find(word[i - 1]);
		if (lpos1->second < lpos2->second)
			return false;
	}

	return true;
}

int alphabet_score(std::vector<std::string> const& words, alphabet_t const& alpabet)
{
	return std::count_if(
		words.begin(), words.end(),
		[&alpabet](std::string const& word) { return is_ordered(word, alpabet); }
	);
}

std::vector<std::string> read_words()
{
	std::ifstream file("..\\words.txt");
	
	if (file.good()) {
		std::istream_iterator<std::string> start(file), end;
		std::vector<std::string> words(start, end);
		return words;
	}
}

void alter_alphabet(alphabet_t & alphabet, std::mt19937 & generator,
	std::uniform_int_distribution<int> & uniform_distribution)
{
	auto l1 = 'A' + uniform_distribution(generator);
	auto l2 = 0;
	do {
		l2 = 'A' + uniform_distribution(generator);
	} while (l2 == l1);

	swap_letter(alphabet, l1, l2);
}

void shuffle_alphabet(alphabet_t & alphabet)
{
	std::vector<alphabet_t::mapped_type> values(alphabet.size());
	std::transform(
		alphabet.begin(), alphabet.end(),
		begin(values),
		[](alphabet_t::value_type const& elem) { return elem.second; }
	);

	std::random_shuffle(values.begin(), values.end());

	auto crtValue = values.begin();
	for (auto &elme : alphabet) {
		elme.second = *crtValue++;
	}
}

std::pair<int, std::string> find_optimal_alphabet(
	std::vector<std::string> const& words, int const timeout)
{
	std::mt19937 rndGenerator(static_cast<unsigned int>
		(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<int> distribution(0, 25);

	auto bestScore = std::make_pair<int, std::string>(0, std::string(""));
	auto alpha = base_alphabet();
	int iteration = 0;

	auto start_time = std::chrono::system_clock::now();

	while (true) {
		auto crt_time = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(crt_time - start_time)
		> std::chrono::seconds(timeout))
		break;

		auto score = alphabet_score(words, alpha);
		if (score > bestScore.first) {
			bestScore.first = score;
			bestScore.second = alphabet_string(alpha);
			std::cout << score << " \t" << alphabet_string(alpha) <<
				" iteration= " << iteration << std::endl;
		}

		alter_alphabet(alpha, rndGenerator, distribution);
		iteration++;

		if ((iteration % 1000) == 0)
			alter_alphabet(alpha, rndGenerator, distribution);
		if ((iteration % 5000) == 0)
			alter_alphabet(alpha, rndGenerator, distribution);
		if ((iteration % 50000) == 0)
			shuffle_alphabet(alpha);
	}

	return bestScore;
}

std::pair<int, std::string> find_optimal_alphabet_async(std::vector<std::string> const& words,
	int const timeout, int const seed) {
	std::mt19937 rndgenerator(seed);
	std::uniform_int_distribution<int> distribution(0, 25);

	auto besescore = std::make_pair<int, std::string>(0, std::string(""));
	auto alpha = base_alphabet();
	shuffle_alphabet(alpha);

	int iteration = 0;

	auto start_time = std::chrono::system_clock::now();

	while (true) {
		auto crt_time = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(crt_time - start_time) >
			std::chrono::seconds(timeout))
			break;

		auto score = alphabet_score(words, alpha);
		if (score > besescore.first) {
			besescore.first = score;
			besescore.second = alphabet_string(alpha);
		}

		alter_alphabet(alpha, rndgenerator, distribution);
		iteration++;

		if ((iteration % 1000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if ((iteration % 5000) == 0)
			alter_alphabet(alpha, rndgenerator, distribution);
		if ((iteration % 50000) == 0)
			shuffle_alphabet(alpha);
	}

	return besescore;
}