#include <iostream>
#include <string>

#include "RecommenderSystem.h"

int main(int argc, char *argv[])
{
//	if (argc < 3)
//	{
//		std::cerr << "invalid usage - expecting at least 6 params" << std::endl;
//		return 1;
//	}

	const std::string EXE = argv[0];
	const std::string MOVIES_FILE = "D:\\Files\\movies_big.txt";//"D:\\Files\\movies_features.txt";
	const std::string RANKS_FILE = "D:\\Files\\ranks_big.txt";//"D:\\Files\\ranks_matrix.txt";
//	const std::string COMMAND_a = argv[1];
//	const std::string COMMAND_b = argv[2];
	const std::string COMMAND =  "cf predict";//COMMAND_a + " " + COMMAND_b;

	RecommenderSystem rec;
	if (rec.loadData(MOVIES_FILE, RANKS_FILE) != 0)
	{
		std::cerr << "Failed loading files" << std::endl;
		return 1;
	}
	if (COMMAND == "content recommend")
	{
//		if (argc != 6)
//		{
//			std::cerr << "invalid usage" << std::endl;
//			return 1;
//		}
		const std::string USER = "Randy";
		string movie = rec.recommendByContent(USER);
		std::cout << movie << std::endl;
	}
	else if (COMMAND == "cf predict")
	{
//		if (argc != 8)
//		{
//			std::cerr << "invalid usage" << std::endl;
//			return 1;
//		}
		const std::string MOVIE = "TheGreatEscape";
		const std::string USER = "Callie";
		const std::string K = "176";
		std::cout << rec.predictMovieScoreForUser(MOVIE, USER, std::stoi(K)) << std::endl;
	}
	else if (COMMAND == "cf recommend")
	{
//		if (argc != 7)
//		{
//			std::cerr << "invalid usage" << std::endl;
//			return 1;
//		}
		const std::string USER = "Brayson";
		const std::string K = "1";
		std::cout << rec.recommendByCF(USER, std::stoi(K)) << std::endl;
	}

	return 0;
}