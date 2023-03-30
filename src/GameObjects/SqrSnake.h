#pragma once

#include "../constants.h"
#include "../util.h"
#include "Enemy.h"
#include "Player.h"

const SDL_Color BEFORE_SNAKE_HEAD = {111, 40, 68};

class SqrSnake : public Enemy {
public:
  struct SnakeProperty {
    int size;
    int len;
  };
  SqrSnake(float startTime, SDL_Point p, float dir = 0, float vel = 5,
           int snakeSize = 50, int snakeLen = 10);
  ~SqrSnake();
  // void move();
  // Check whether it collide the player
  bool checkCollision(Player *p);
  void loadMedia();
  // Show the snake on the screen
  void render();

private:
  const float splash_gap = 100;
  const float splash_delay = 10;
  float lastSplash;
  int gapSqr;
  SnakeProperty s;
  // Square snake's collision box
  SDL_Rect sCollider;
};
