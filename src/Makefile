izvrsni: main.cpp solver.o solution.o
	g++ -std=c++11 main.cpp solver.o solution.o -o izvrsni
solver.o: solver.cpp solver.hpp
	g++ -std=c++11 -c solver.cpp
solution.o: solution.cpp solution.hpp
	g++ -std=c++11 -c solution.cpp

.PHONY: clean
clean:
	rm -rf *.o izvrsni 