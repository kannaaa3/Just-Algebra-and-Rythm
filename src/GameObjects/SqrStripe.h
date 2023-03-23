#pragma once

#include "../constants.h"
#include "../util.h"

enum SqrStripeState { IDLE = 0, SPLASH, APPEAR, DISAPPEAR };

class SqrStripe {
public:
  // Time on the screen (millisecond)
  const float APPEAR_TIME = 100;
  SqrStripe();
  ~SqrStripe();

  bool checkCollision();
  void renderCenter();

private:
  // Square size
  int size;
  int posX, posY;
  SqrStripeState state;
  vector<int> numFrame = {1, 1, 1};
  SDL_Rect collider;
};
