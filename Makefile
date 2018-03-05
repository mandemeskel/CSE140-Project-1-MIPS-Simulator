tests : computer.o tests.o
	gcc -g -Wall -o tests tests.o computer.o

tests.o : computer.h tests.c
	gcc -g -c -Wall tests.c

sim : computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

clean:
	\rm -rf *.o sim
