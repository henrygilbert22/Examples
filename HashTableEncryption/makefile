.PHONY: all
all: hashTester userpass

hashTester: hashTester.o 
	g++ -o hashTester -std=c++11 hashTester.o 
hashTester.o: hashTester.cpp hashtable.hpp 
	g++ -c -std=c++11 hashTester.cpp

userpass: userpass.o pass_server.o
	g++ -o userpass -std=c++11 userpass.o pass_server.o 
userpass.o: userpass.cpp hashtable.hpp 
	g++ -c -std=c++11 userpass.cpp

pass_server.o: pass_server.h pass_server.cpp  hashtable.hpp
	g++ -c -std=c++11 pass_server.h
	g++ -c -std=c++11 pass_server.cpp -lcrypt 



clean:
	-rm *.o
	-rm *.h.gch
