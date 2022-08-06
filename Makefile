CXX      = g++
CXXFLAGS = -std=c++17 -O3
LDLIBS   = -lm
PROGRAM  = simulated_annealing

all : program

program : main.o algorithms.o util.o tests.o
	$(CXX) $^ -o $(PROGRAM) $(LDLIBS)

main.o : main.cpp tests.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests.o : tests.cpp tests.hpp util.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

util.o : util.cpp util.hpp algorithms.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

algorithms.o : algorithms.cpp algorithms.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm *.o $(PROGRAM)