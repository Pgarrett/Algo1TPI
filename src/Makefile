
all: main

clean:
	rm -f *.o
	rm -f main

main: main.cpp tipos.h campo.o drone.o sistema.o
	g++ -g -Wall -o main main.cpp campo.o drone.o sistema.o

campo.o: campo.h campo.cpp
	g++ -g -Wall -c campo.cpp

drone.o: drone.h drone.cpp
	g++ -g -Wall -c drone.cpp

sistema.o: sistema.h sistema.cpp
		g++ -g -Wall -c sistema.cpp
