CPP_FILES = src/main.cpp src/SDL_utils.cpp src/util.cpp src/Levels/Level.cpp  src/GameObjects/Renderable.cpp src/Menu/Menu.cpp src/Menu/Setting.cpp src/Menu/GameOver.cpp src/Menu/Transition.cpp src/Menu/PageManager.cpp src/Menu/LevelSelect.cpp
#src/main.cpp  src/Menu/PageManager.cpp src/Menu/LevelSelect.cpp src/GameObjects/Player.cpp src/Levels/Level.cpp src/util.cpp

#src/Levels/Level.cpp src/GameObjects/Player.cpp src/Menu/Menu.cpp src/Menu/GameOver.cpp src/Menu/Transition.cpp src/Menu/PageManager.cpp

#src/Menu/PageManager.cpp src/Levels/Level.cpp src/Menu/GameOver.cpp

TMP_FILES = src/SDL_utils.cpp src/util.cpp src/GameObjects/MovingEntity.cpp src/GameObjects/SqrSnake.cpp src/Levels/Level.cpp src/GameObjects/Player.cpp src/GameObjects/Renderable.cpp src/GameObjects/BouncingEntity.cpp src/GameObjects/Enemy.cpp src/Menu/Menu.cpp src/Menu/Setting.cpp src/Menu/GameOver.cpp src/Menu/Transition.cpp src/Menu/PageManager.cpp src/Menu/LevelSelect.cpp
OBJS = out/util.o out/SDL_utils.o out/main.o out/Player.o out/MovingEntity.o out/Level.o out/Enemy.o out/SqrSnake.o out/BouncingEntity.o out/Entity.o out/Renderable.o out/Menu.o out/Transition.o out/PageManager.o out/Setting.o out/GameOver.o out/LevelSelect.o

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2

EXECUTABLE = main


all: $(CPP_FILES)
	$(CC) -c $(LINKER_FLAGS) $(CPP_FILES) 
	mv *.o out
	$(CC) $(LINKER_FLAGS) $(OBJS) -o $(EXECUTABLE)
	./main
clean:
	mv *.o out

main.o: src/main.cpp
	$(CC) -c $(LINKER_FLAGS) src/main.cpp

SDL_util.o: src/SDL_util.cpp src/SDL_util.h
	$(CC) -c $(LINKER_FLAGS) src/SDL_util.cpp
util.o: src/util.cpp src/util.h
	$(CC) -c $(LINKER_FLAGS) src/util.cpp
Player.o: src/GameObjects/Player.cpp src/GameObjects/Player.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/Player.cpp
Enemy.o: src/GameObjects/Enemy.cpp src/GameObjects/Enemy.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/Enemy.cpp
MovingEntity.o: src/GameObjects/MovingEntity.cpp src/GameObjects/MovingEntity.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/MovingEntity.cpp
SqrSnake.o: src/GameObjects/SqrSnake.cpp src/GameObjects/SqrSnake.h
	$(CC) -c $(LINKER_FLAGS) src/GameObjects/SqrSnake.cpp
Level.o: src/Levels/Level.cpp src/Levels/Level.h
	$(CC) -c $(LINKER_FLAGS) src/Levels/Level.cpp
#
# CPP_FILES = src/main.cpp src/Menu/PageManager.cpp src/Menu/LevelSelect.cpp src/Menu/GameOver.cpp src/Levels/Level.cpp src/Menu/Menu.cpp src/GameObjects/Player.cpp
#
# SRC_FILES = $(wildcard src/*.cpp src/*/*.cpp)
# OBJ_FILES = $(patsubst %.cpp, out/%.o, $(SRC_FILES))
#
#
# #src/Menu/PageManager.cpp src/Levels/Level.cpp src/Menu/GameOver.cpp
#
# TMP_FILES = src/SDL_utils.cpp src/util.cpp src/GameObjects/MovingEntity.cpp src/GameObjects/SqrSnake.cpp src/Levels/Level.cpp src/GameObjects/Player.cpp src/GameObjects/Renderable.cpp src/GameObjects/BouncingEntity.cpp src/GameObjects/Enemy.cpp src/Menu/Menu.cpp src/Menu/Setting.cpp src/Menu/GameOver.cpp src/Menu/Transition.cpp src/Menu/PageManager.cpp src/Menu/LevelSelect.cpp
# OBJS = out/util.o out/SDL_utils.o out/main.o out/Player.o out/MovingEntity.o out/Level.o out/Enemy.o out/SqrSnake.o out/BouncingEntity.o out/Entity.o out/Renderable.o out/Menu.o out/Transition.o out/PageManager.o out/Setting.o out/GameOver.o out/LevelSelect.o
#
# CC = g++
#
# COMPILER_FLAGS = -w
#
# LINKER_FLAGS = -I/usr/include/SDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2
#
# EXECUTABLE = main
#
# all: main
#
# clean:
# 	rm out/*.o
#
# main: $(OBJ_FILES)
# 	$(CC) $(LINKER_FLAGS) $(OBJ_FILES) -o $(EXECUTABLE)
#
# out/%.o: src/%.cpp src/*/%.cpp
# 	$(CC) -c $(LINKER_FLAGS) $@ -c $<

