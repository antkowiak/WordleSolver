ws : main.cpp WordleSolver.h WordleSolver.cpp Makefile
	g++ -g -std=c++17 main.cpp WordleSolver.cpp -o ws

clean :
	\rm -f ws
	