#
#
#	Makefile for test programs
#
#
RUNMO = ./monitor
RUNSI = ./simulador

#C compiler to use
CC = gcc
#List of flags to pass to the compilation command
#CFLAGS = -g
#Bibliotecas
#LIBS = -lns -lnsl
#LIBS = -lm -lnsl
LIBS = -lpthread
#LIBS = -lsocket -lnsl -lns
#LIBS = -lsocket -lnsl -lns

all: clean monitor simulador runm

monitor: unix-monitor.o util.o monitor.o
	$(CC) -g -o monitor unix-monitor.o util.o monitor.o $(LIBS)

simulador: unix-simulador.o util.o simulador.o
	$(CC) -g -o simulador unix-simulador.o util.o simulador.o $(LIBS)

runm:
		x-terminal-emulator --geometry "5x5" -e "bash -c $(RUNSI);bash"; resize -s 37 80;$(RUNMO);

clean:
	rm -f *.o *~ monitor simulador
