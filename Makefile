CC= g++
CFLAGS= -O2 -Wall -fstack-protector-all -fcf-protection=full
LDLIBS= -lws2_32
program: main.cpp
	${CC} ${CFLAGS} -o program main.cpp ${LDLIBS}