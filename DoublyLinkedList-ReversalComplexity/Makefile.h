CC := g++

#CFLAGS := -std=c++03
#CFLAGS := -std=c++98
#CFLAGS := -std=c++0x

CFLAGS := -std=c++11 

SRCS := test_list.cpp measure_reverse.cpp 

OBJS := ${SRCS:.cpp=.o} 

PROGS := ${SRCS:.cpp=}

.PHONY: all
all: ${PROGS}

test_list: test_list.o
	${CC} ${CFLAGS} -o $@ $^

measure_reverse: measure_reverse.o
	${CC} ${CFLAGS} -o $@ $^

%.o: %.cpp List.hpp
	${CC} ${CFLAGS} -c $<

clean:
	rm -f ${PROGS} ${OBJS} *.bak *~
