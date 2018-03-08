testExecute : helper.o execute.o testExecute.o
	gcc -g -Wall -o testExecute testExecute.o execute.o helper.o

testExecute.o : testExecute.c
	gcc -g -c -Wall testExecute.c

testDecode : helper.o decode.o testDecode.o
	gcc -g -Wall -o testDecode testDecode.o decode.o helper.o

testDecode.o : testDecode.c
	gcc -g -c -Wall testDecode.c

sim : helper.o updatepc.o execute.o decode.o computer.o sim.o
	gcc -g -Wall -o sim sim.o computer.o decode.o execute.o updatepc.o helper.o

sim.o : computer.h sim.c
	gcc -g -c -Wall sim.c

computer.o : computer.c computer.h
	gcc -g -c -Wall computer.c

decode.o : decode.c decode.h
	gcc -g -c -Wall decode.c

execute.o : execute.c execute.h
	gcc -g -c -Wall execute.c

updatepc.o : updatepc.c updatepc.h
	gcc -g -c -Wall updatepc.c

helper.o : helper.c helper.h
	gcc -g -c -Wall helper.c

clean:
	\rm -rf *.o sim testDecode
