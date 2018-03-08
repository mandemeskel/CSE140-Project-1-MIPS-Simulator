testExecute : helper.o execute.o decode.o computer.o testExecute.o
	gcc -g -Wall -o testExecute testExecute.o computer.o decode.o execute.o helper.o

testExecute.o : testExecute.c
	gcc -g -c -Wall testExecute.c

testDecode : helper.o decode.o computer.o testDecode.o
	gcc -g -Wall -o testDecode testDecode.o computer.o decode.o execute.o helper.o

testDecode.o : testDecode.c
	gcc -g -c -Wall testDecode.c

sim : execute.o helper.o decode.o computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o decode.o execute.o helper.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

decode.o : decode.c decode.h
	gcc -g -c -Wall decode.c

execute.o : execute.c execute.h
	gcc -g -c -Wall execute.c

helper.o : helper.c helper.h
	gcc -g -c -Wall helper.c

clean:
	\rm -rf *.o sim testDecode
