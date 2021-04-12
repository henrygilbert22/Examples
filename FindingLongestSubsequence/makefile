.PHONY: all
all: hg19b_driver LongestSubseq TestCell TestSeq

hg19b_driver: hg19b_driver.o Cell.o
	g++ -o hg19b_driver -std=c++11 hg19b_driver.o  Cell.o
hg19b_driver.o: hg19b_driver.cpp NSequence.hpp FunClassTemp.hpp
	g++ -c -std=c++11 hg19b_driver.cpp
Cell.o:  Cell.h Cell.cpp
	g++ -c -std=c++11 Cell.h
	g++ -c -std=c++11 Cell.cpp

LongestSubseq: LongestSubseq.o Cell.o 
	g++ -o LongestSubseq -std=c++11 LongestSubseq.o  Cell.o 
LongestSubseq.o: LongestSubseq.cpp NSequence.hpp FunClassTemp.hpp
	g++ -c -std=c++11 LongestSubseq.cpp
Cell.o:  Cell.h Cell.cpp
	g++ -c -std=c++11 Cell.h
	g++ -c -std=c++11 Cell.cpp 

TestCell: Cell.o TestCell.o
	g++ -c TestCell -std=c++11 Cell.cpp TestCell.cpp
TestCell.o: TestCell.cpp
	g++ -c -std=c++11 TestCell.cpp
Cell.o: Cell.h Cell.cpp
	g++ -c -std=c++11 Cell.h
	g++ -c -std=c++11 Cell.cpp

TestSeq: TestSeq.o Cell.o
	g++ -o TestSeq -std=c++11 TestSeq.o  Cell.o
TestSeq.o: TestSeq.cpp NSequence.hpp
	g++ -c -std=c++11 TestSeq.cpp
Cell.o:  Cell.h Cell.cpp
	g++ -c -std=c++11 Cell.h
	g++ -c -std=c++11 Cell.cpp




clean:
	-rm *.o
	-rm *.h.gch
