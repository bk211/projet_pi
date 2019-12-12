CC=g++
CFLAGS= -Wall -O3
LIBS= -pthread -lgmp -lgmp
nb_thread = 4
precision = 100000
exec:	pi.cpp pi.hpp
	g++ $< -o $@ $(CFLAGS) $(LIBS)

run:	exec
	./exec $(nb_thread) $(precision) 
clean:
	rm exec 