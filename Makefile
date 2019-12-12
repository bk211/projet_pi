exec:	pi.cpp
	g++ $< -Wall -o $@ -pthread -lgmpxx -lgmp