CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
all: demo tests

demo: chess_movement.cc demo.cc chess_movement.h
	$(CXX) $(CXXFLAGS) chess_movement.cc demo.cc -o demo

tests: chess_movement.cc tests.cc chess_movement.h
	$(CXX) $(CXXFLAGS) chess_movement.cc tests.cc -o tests

check: tests
	./tests
clean:
	rm -f demo tests
.PHONY: all check clean
