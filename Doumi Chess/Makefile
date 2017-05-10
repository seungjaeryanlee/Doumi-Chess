# Compiler
CC = g++
CPPFLAGS = -std=c++0x

all: doumiUCI
	
doumiUCI: uci.o evaluation.o movegen.o move.o board.o
	$(CC) $(CPPFLAGS) uci.o evaluation.o movegen.o move.o board.o -o doumiUCI

doumiTEST: debugmain.o debug.o evaluation.o movegen.o move.o board.o timer.o
	$(CC) $(CPPFLAGS) debugmain.o debug.o evaluation.o movegen.o move.o board.o timer.o -o doumiTEST

debugmain.o: debugmain.cpp debug.h
	$(CC) $(CPPFLAGS) -c debugmain.cpp

uci.o: uci.cpp evaluation.h board.h
	$(CC) $(CPPFLAGS) -c uci.cpp

debug.o: debug.cpp debug.h movegen.h
	$(CC) $(CPPFLAGS) -c debug.cpp

evaluation.o: evaluation.cpp evaluation.h movegen.h
	$(CC) $(CPPFLAGS) -c evaluation.cpp

movegen.o: movegen.cpp movegen.h board.h
	$(CC) $(CPPFLAGS) -c movegen.cpp

board.o: board.cpp board.h
	$(CC) $(CPPFLAGS) -c board.cpp

move.o: move.cpp move.h
	$(CC) $(CPPFLAGS) -c move.cpp

timer.o: timer.cpp timer.h
	$(CC) $(CPPFLAGS) -c timer.cpp	

clean:
	rm -f *.o doumiUCI doumiTEST
