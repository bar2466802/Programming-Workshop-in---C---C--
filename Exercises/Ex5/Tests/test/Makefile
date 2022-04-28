CLI = ./cli.cpp

RecommenderSystem.o:
	g++ -Wall -Wvla -Wextra -Werror -g -std=c++17 ./RecommenderSystem.cpp -c

compile: RecommenderSystem.o
	g++ -Wall -Wvla -Wextra -Werror -g -std=c++17 $(CLI) RecommenderSystem.o -o prog

clean:
	rm -rf *.o
	rm -rf prog