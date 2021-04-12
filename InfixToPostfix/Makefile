.PHONY: all
all: test_stack1 


CC := g++

CFLAGS := -std=c++11 

SRCS := in2post.cpp 

OBJS := ${SRCS:.cpp=.o} opnum.o

PROGS := ${SRCS:.cpp=} 

.PHONY: all
all: ${PROGS}

in2post: in2post.o opnum.o
	${CC} ${CFLAGS} $^ -o $@ -ll

opnum.cpp: opnum.fl
	flex -o opnum.cpp opnum.fl
	
%.o: %.cpp
	${CC} ${CFLAGS} -c $<


test_stack1: test_stack1.o 
	g++ -o test_stack1 -std=c++11 test_stack1.o 
test_stack1.o: test_stack1.cpp stack.hpp 
	g++ -c -std=c++11 test_stack1.cpp

clean:
	rm -f ${PROGS} opnum.cpp ${OBJS} *.bak *~
