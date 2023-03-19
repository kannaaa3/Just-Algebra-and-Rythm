#pragma once

#include "../util.h"
#include "../constants.h"


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

