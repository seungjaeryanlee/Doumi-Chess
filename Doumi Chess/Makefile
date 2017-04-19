# Compiler
CC = g++
CPPFLAGS = -std=c++0x

all: nageuneuci
	
nageuneuci: uci.o evaluation.o movegen.o move.o board.o
	$(CC) $(CPPFLAGS) uci.o evaluation.o movegen.o move.o board.o -o nageuneuci

uci.o: uci.cpp evaluation.h board.h
	$(CC) $(CPPFLAGS) -c uci.cpp

evaluation.o: evaluation.cpp evaluation.h movegen.h
	$(CC) $(CPPFLAGS) -c evaluation.cpp

movegen.o: movegen.cpp movegen.h board.h
	$(CC) $(CPPFLAGS) -c movegen.cpp

board.o: board.cpp board.h
	$(CC) $(CPPFLAGS) -c board.cpp

move.o: move.cpp move.h
	$(CC) $(CPPFLAGS) -c move.cpp

clean:
	rm -f *.o nageuneuci
