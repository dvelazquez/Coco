# Makefile to compile the "Measurement and Control Computarized System"
# Daniel Velazquez  <dvelazquezb@gmail.com>
#-----------
# GNU/Linux compiler
MvBin= mv Coco.bin bin
MvObj= mv *.o tmp
CC=gcc 
CFLAGS= -Wall -Wno-unused -O3
ALLEGRO= `/usr/local/bin/allegro-config --libs`

all: Coco.bin

Coco.bin: Main.o Register.o Manual.o Automatic.o Simulator.o Help.o Validate.o Close.o
	$(CC) -o $@ Main.o Register.o Manual.o Automatic.o Simulator.o Help.o Validate.o Close.o $(ALLEGRO)
	$(MvBin)
	$(MvObj)
Main.o: src/Main.c src/IR.h
	$(CC) $(CFLAGS) -c src/Main.c $(ALLEGRO)

Register.o: src/Register.c src/IR.h
	$(CC) $(CFLAGS) -c src/Register.c $(ALLEGRO)

Manual.o: src/Manual.c src/IR.h
	$(CC) $(CFLAGS) -c src/Manual.c $(ALLEGRO)

Automatic.o: src/Automatic.c src/IR.h
	$(CC) $(CFLAGS) -c src/Automatic.c $(ALLEGRO)

Simulator.o: src/Simulator.c src/IR.h
	$(CC) $(CFLAGS) -c src/Simulator.c $(ALLEGRO)

Help.o: src/Help.c src/IR.h
	$(CC) $(CFLAGS) -c src/Help.c $(ALLEGRO)	

Validate.o: src/Validate.c
	$(CC) $(CFLAGS) -c src/Validate.c $(ALLEGRO)	

Close.o: src/Close.c
	$(CC) $(CFLAGS) -c src/Close.c $(ALLEGRO)		
