testDecode : helper.o decode.o computer.o testDecode.o
	gcc -g -Wall -o testDecode testDecode.o computer.o decode.o helper.o

testDecode.o : testDecode.c
	gcc -g -c -Wall testDecode.c

sim : helper.o decode.o computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o decode.o helper.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

decode.o : decode.c decode.h
	gcc -g -c -Wall decode.c

helper.o : helper.c helper.h
	gcc -g -c -Wall helper.c

clean:
	\rm -rf *.o sim testDecode
