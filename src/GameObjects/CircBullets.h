#pragma once

#include "../constants.h"
#include "../util.h"

class CircBullets {
public:
  CircBullets();
  ~CircBullets();

  bool checkCollision();
  void render();

private:
  float angleVel;
  float timeAppear;
  int numStripe;
  int stripeW;
};
