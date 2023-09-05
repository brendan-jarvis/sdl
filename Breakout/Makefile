CC = g++
CFLAGS = -std=c17 -Wall
INCLUDES = -I /Library/Frameworks/SDL2.framework/Headers -I /Library/Frameworks/SDL2_ttf.framework/Headers -I /Library/Frameworks/SDL2_image.framework/Headers
LINKS = -F/Library/Frameworks
LINK_FLAGS = -framework SDL2 -framework SDL2_ttf -framework SDL2_image
APP = breakout
SRC_FILES = main.cpp
OBJ_FILES = main.o
.c.o: $(SRC_FILES)
	$(CC) $(CFLAGS) ${INCLUDES} -c $^
all: $(OBJ_FILES)
	$(CC) $(CFLAGS) ${LINKS} -o $(APP) $< $(LINK_FLAGS)
clean:
	rm -rf $(OBJ_FILES) $(APP)
