CPP_FILES = main.cpp GameObjects/MovingEntity.cpp GameObjects/Enemy.cpp
TMP_FILES = SDL_utils.cpp util.cpp GameObjects/SpinningRect.cpp GameObjects/Player.cpp Levels/Level.cpp
OBJS = SDL_utils.o util.o main.o Player.o SpinningRect.o MovingEntity.o Level.o Enemy.o
OBJ_FILES = out/*.o # src/main.c -> out/main.o


CC = g++

OBJDIR = out

COMPILER_FLAGS = -w

LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

EXECUTABLE = main


all: $(CPP_FILES)
	$(CC) -c $(LINKER_FLAGS) $(CPP_FILES) 
	mv *.o out
	$(CC) $(LINKER_FLAGS) $(OBJ_FILES) -o $(EXECUTABLE)

	# $(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
clean:
	rm out/*.o main
