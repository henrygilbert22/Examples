.PHONY: all
all: test_list measure

test_list: test_list.o 
	g++ -o test_list -std=c++11 test_list.o 
test_list.o: test_list.cpp List.hpp 
	g++ -c -std=c++11 test_list.cpp

measure: measure.o 
	g++ -o measure -std=c++11 measure.o 
measure.o: measure.cpp List.hpp 
	g++ -c -std=c++11 measure.cpp



clean:
	-rm *.o
	-rm *.h.gch