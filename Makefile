all: main

CC=gcc
CFLAGS=-g -Wall
INCLUDE=include
LD=-lSDL -lGL -lGLEW
LIB=-Llib
SRC=src
OBJ=main.o mesh.o program.o image.o material.o model.o camera.o light.o\
	texture.o stack_allocator.o list.o pool_allocator.o stack.o\
	map.o array.o error.o scene_manager.o light_manager.o

main: $(OBJ)
	$(CC) -o main $(OBJ) $(LD)

main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $(SRC)/main.c -I$(INCLUDE)

mesh.o: $(INCLUDE)/mesh.h $(SRC)/mesh.c
	$(CC) $(CFLAGS) -c $(SRC)/mesh.c -I$(INCLUDE)

program.o: $(INCLUDE)/program.h $(SRC)/program.c
	$(CC) $(CFLAGS) -c $(SRC)/program.c -I$(INCLUDE)

image.o: $(INCLUDE)/image.h $(SRC)/image.c
	$(CC) $(CFLAGS) -c $(SRC)/image.c -I$(INCLUDE)

material.o: $(INCLUDE)/material.h $(SRC)/material.c
	$(CC) $(CFLAGS) -c $(SRC)/material.c -I$(INCLUDE)

model.o: $(INCLUDE)/model.h $(SRC)/model.c
	$(CC) $(CFLAGS) -c $(SRC)/model.c -I$(INCLUDE)

camera.o: $(INCLUDE)/camera.h $(SRC)/camera.c
	$(CC) $(CFLAGS) -c $(SRC)/camera.c -I$(INCLUDE)

light.o: $(INCLUDE)/light.h $(SRC)/light.c
	$(CC) $(CFLAGS) -c $(SRC)/light.c -I$(INCLUDE)

texture.o: $(INCLUDE)/texture.h $(SRC)/texture.c
	$(CC) $(CFLAGS) -c $(SRC)/texture.c -I$(INCLUDE)

stack_allocator.o: $(INCLUDE)/stack_allocator.h $(SRC)/stack_allocator.c
	$(CC) $(CFLAGS) -c $(SRC)/stack_allocator.c -I$(INCLUDE)

list.o: $(INCLUDE)/list.h $(SRC)/list.c
	$(CC) $(CFLAGS) -c $(SRC)/list.c -I$(INCLUDE)

pool_allocator.o: $(INCLUDE)/pool_allocator.h $(SRC)/pool_allocator.c
	$(CC) $(CFLAGS) -c $(SRC)/pool_allocator.c -I$(INCLUDE)

stack.o: $(INCLUDE)/stack.h $(SRC)/stack.c
	$(CC) $(CFLAGS) -c $(SRC)/stack.c -I$(INCLUDE)

map.o: $(INCLUDE)/map.h $(SRC)/map.c
	$(CC) $(CFLAGS) -c $(SRC)/map.c -I$(INCLUDE)

array.o: $(INCLUDE)/array.h $(SRC)/array.c
	$(CC) $(CFLAGS) -c $(SRC)/array.c -I$(INCLUDE)

error.o: $(INCLUDE)/error.h $(SRC)/error.c
	$(CC) $(CFLAGS) -c $(SRC)/error.c -I$(INCLUDE)

scene_manager.o: $(INCLUDE)/scene_manager.h $(SRC)/scene_manager.c
	$(CC) $(CFLAGS) -c $(SRC)/scene_manager.c -I$(INCLUDE)

light_manager.o: $(INCLUDE)/light_manager.h $(SRC)/light_manager.c
	$(CC) $(CFLAGS) -c $(SRC)/light_manager.c -I$(INCLUDE)

clean:
	-rm main $(OBJ)
