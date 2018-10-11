CC=g++
CFLAGS=-Wall -I../../include/
LDFLAGS=-L../../../sbmlsolver_build/lib/ -lsbmlsolver -lc++

test: test.o
	$(CC) $(CFLAGS) -o test test.o $(LDFLAGS)


test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp

