CC = g++
CFLAGS = -std=c17 -Wall
INCLUDES = -I /usr/local/Cellar/sdl2/2.28.2/include
LINKS = -L /usr/local/Cellar/sdl2/2.28.2/lib
LINK_FLAGS = -lSDL2
APP = asteroids
SRC_FILES = main.cpp
OBJ_FILES = main.o
.c.o: $(SRC_FILES)
	$(CC) $(CFLAGS) ${INCLUDES} -c $^
all: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(APP) ${LINKS} $< $(LINK_FLAGS)
clean:
	rm -rf $(OBJ_FILES) $(APP)