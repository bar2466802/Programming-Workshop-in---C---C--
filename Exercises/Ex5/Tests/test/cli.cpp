#include <iostream>
#include <string>

#include "RecommenderSystem.h"

int main(int argc, char *argv[])
{
	if (argc < 6)
	{
		std::cerr << "invalid usage - expecting at least 6 params" << std::endl;
		return 1;
	}

	const std::string EXE = argv[0];
	const std::string MOVIES_FILE = argv[1];
	const std::string RANKS_FILE = argv[2];
	const std::string COMMAND_a = argv[3];
	const std::string COMMAND_b = argv[4];
	const std::string COMMAND = COMMAND_a + " " + COMMAND_b;

	RecommenderSystem rec;
	if (rec.loadData(MOVIES_FILE, RANKS_FILE) != 0)
	{
		std::cerr << "Failed loading files" << std::endl;
		return 1;
	}
	if (COMMAND == "content recommend")
	{
		if (argc != 6)
		{
			std::cerr << "invalid usage" << std::endl;
			return 1;
		}
		const std::string USER = argv[5];
		std::cout << rec.recommendByContent(USER) << std::endl;
	}
	else if (COMMAND == "cf predict")
	{
		if (argc != 8)
		{
			std::cerr << "invalid usage" << std::endl;
			return 1;
		}
		const std::string MOVIE = argv[5];
		const std::string USER = argv[6];
		const std::string K = argv[7];
		std::cout << rec.predictMovieScoreForUser(MOVIE, USER, std::stoi(K)) << std::endl;
	}
	else if (COMMAND == "cf recommend")
	{
		if (argc != 7)
		{
			std::cerr << "invalid usage" << std::endl;
			return 1;
		}
		const std::string USER = argv[5];
		const std::string K = argv[6];
		std::cout << rec.recommendByCF(USER, std::stoi(K)) << std::endl;
	}

	return 0;
}