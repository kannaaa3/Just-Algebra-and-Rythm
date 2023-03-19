#pragma once

#include "../util.h"
#include "../constants.h"

const SDL_Color BEFORE_SNAKE_HEAD = {111, 40, 68};

enum SqrSnakeState {
  MOVE = 0,
  SUB_MOVE
};

class SqrSnake {
public:
  // The gap between squares (a.k.a snake's body)
  const int GAP_SQUARE = 2;

  SqrSnake();
  ~SqrSnake();
  void move();
  // Check whether it collide the player
  bool checkCollision();
  // Show the snake on the screen
  void render();
private:
  // postion of the first square
  int posX, posY;
  // radius of square
  int size;
  // number of square
  int length;
  // velocity of snake
  int vel;
  // Square snake's collision box
  SDL_Rect sCollider;
};

