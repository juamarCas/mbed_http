GXX     = gcc
INCLUDE = ./include

all: main.o
	$(GXX) -o app main.o 

debug: main.o
	$(GXX) -o app main.o -g

main.o: 
	$(GXX) -c main.c -I$(INCLUDE) -g
clean:
	rm -f app *.o
