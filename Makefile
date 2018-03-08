testDecode : decode.o computer.o testDecode.o
	gcc -g -Wall -o testDecode testDecode.o computer.o decode.o

testDecode.o : testDecode.c
	gcc -g -c -Wall testDecode.c

sim : decode.o computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o decode.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

decode.o : decode.c decode.h
	gcc -g -c -Wall decode.c

clean:
	\rm -rf *.o sim testDecode
