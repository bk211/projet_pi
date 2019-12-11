exec:	test.cpp
	g++ $< -Wall -o $@ -lpthread -lgmp -lgmpxx