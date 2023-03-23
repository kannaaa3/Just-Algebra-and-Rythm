CPP_FILES = src/main.cpp src/GameObjects/SqrSnake.cpp
TMP_FILES = src/SDL_utils.cpp src/util.cpp src/GameObjects/SpinningRect.cpp src/GameObjects/Player.cpp src/Levels/Level.cpp src/GameObjects/Enemy.cpp src/GameObjects/MovingEntity.cpp
OBJS = SDL_utils.o util.o main.o Player.o SpinningRect.o MovingEntity.o Level.o Enemy.o SqrSnake.o
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
