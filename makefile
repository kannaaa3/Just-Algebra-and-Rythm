CPP_FILES = src/main.cpp src/GameObjects/BouncingEntity.cpp src/GameObjects/Player.cpp
#src/GameObjects/MRenderable.cpp
TMP_FILES = src/SDL_utils.cpp src/util.cpp src/GameObjects/MovingEntity.cpp src/GameObjects/SqrSnake.cpp src/Levels/Level.cpp 
OBJS = out/SDL_utils.o out/util.o out/main.o out/Player.o out/MovingEntity.o out/Level.o out/Enemy.o out/SqrSnake.o out/BouncingEntity.o out/Entity.o out/Renderable.o
OBJ_FILES = out/*.o # src/main.c -> out/main.o

CC = g++

OBJDIR = out

COMPILER_FLAGS = -w

LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

EXECUTABLE = main


all: $(CPP_FILES)
	$(CC) -c $(LINKER_FLAGS) $(CPP_FILES) 
	mv *.o out
	$(CC) $(LINKER_FLAGS) $(OBJS) -o $(EXECUTABLE)
clean:
	mv *.o out

out/main.o: src/main.cpp
	$(CC) -c $(LINKER_FLAGS) src/main.cpp

out/SDL_util.o: src/SDL_util.cpp src/SDL_util.h
	$(CC) -c $(LINKER_FLAGS) src/SDL_util.cpp
out/util.o: src/util.cpp src/util.h
	$(CC) -c $(LINKER_FLAGS) src/util.cpp
out/Player.o: src/GameObjects/Player.cpp src/GameObjects/Player.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/Player.cpp
out/Enemy.o: src/GameObjects/Enemy.cpp src/GameObjects/Enemy.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/Enemy.cpp
out/MovingEntity.o: src/GameObjects/MovingEntity.cpp src/GameObjects/MovingEntity.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/MovingEntity.cpp
out/SqrSnake.o: src/GameObjects/SqrSnake.cpp src/GameObjects/SqrSnake.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/SqrSnake.cpp
out/Level.o: src/Levels/Level.cpp src/Levels/Level.h
	$(CC) -c $(LINKER_FLAGS) src/Levels/Level.cpp

