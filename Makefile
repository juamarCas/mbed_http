GXX     = gcc
TARGET  = bin/app
SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c, obj/%.o, $(SRC))

default: $(TARGET)

$(TARGET): $(OBJ)
	$(GXX) -o $@ $? -g 

obj/%.o: src/%.c
	$(GXX) -c $< -o $@ -Iinclude -g

clean:
	rm -f obj/*.o
	rm -f bin/*
