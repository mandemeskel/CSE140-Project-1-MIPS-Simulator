testDecode : computer.o testDecode.o
	gcc -g -Wall -o testDecode testDecode.o computer.o

testDecode.o : computer.h testDecode.c
	gcc -g -c -Wall testDecode.c

sim : computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

clean:
	\rm -rf *.o sim
