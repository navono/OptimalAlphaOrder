#include <map>
#include <assert.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <iterator>
#include <fstream>
#include <utility>
#include <future>

#include "..\OptimalAlphaOrderCore\OptimalAlphaOrderCore.h"
#pragma comment(lib, "..\\Debug\\OptimalAlphaOrderCore")


void find_optimal_alphabet_sequential(std::vector<std::string> const & words,
	int const timeout) {
	auto bestscore = find_optimal_alphabet(words, timeout);

	std::cout << "best score:" << std::endl;
	std::cout << bestscore.first << " \t" << bestscore.second << std::endl;
}

void find_optimal_alphabet_parallel(std::vector<std::string> const & words,
	int const timeout, int const taskscount) {
	std::vector<std::future<std::pair<int, std::string>>> futures(taskscount);

	unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
	unsigned int index = 0;
	for (auto & f : futures)
	{
		f = std::async(std::launch::async, [&](){return find_optimal_alphabet_async(words, timeout, seed + index++); });
	}

	auto bestscore = std::make_pair<int, std::string>(0, std::string(""));
	for (auto & f : futures)
	{
		auto score = f.get();
		std::cout << score.first << " \t" << score.second << std::endl;
		if (score.first > bestscore.first)
			bestscore = score;
	}

	std::cout << "best score:" << std::endl;
	std::cout << bestscore.first << " \t" << bestscore.second << std::endl;
}

int main() {
	find_optimal_alphabet_sequential(read_words(), 60);

	std::cin.get();
	return 0;
}
