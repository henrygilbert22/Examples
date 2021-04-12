
ILoveWhalley: ILoveWhalley.o cache.o
	g++ -o main -std=c++11 ILoveWhalley.o 

ILoveWhalley.o: ILoveWhalley.cpp cache.cpp 
	g++ -c -std=c++11 ILoveWhalley.cpp

cache.o: cache.h cache.cpp 
	g++ -c -std=c++11 cache.cpp 

clean:
	-rm *.o
	-rm *.h.gch
