OBJ_FILES = SDL_utils.cpp util.cpp main.cpp GameObjects/Player.cpp  GameObjects/SpinningRect.cpp GameObjects/MovingEntity.cpp
OBJS = SDL_utils.o util.o main.o Player.o SpinningRect.o MovingEntity.o

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

EXECUTABLE = main

all: $(OBJ_FILES)
	$(CC) -c $(LINKER_FLAGS) $(OBJ_FILES) 
	$(CC) $(LINKER_FLAGS) $(OBJS) -o $(EXECUTABLE)

	# $(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
clean:
	rm *.o main
