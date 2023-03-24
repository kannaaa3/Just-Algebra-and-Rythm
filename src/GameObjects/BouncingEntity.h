#pragma once
#include "../constants.h"

// The entity will bounce by going down and back
// Delay a small time before the next bounce
class BouncingEntity {
public:
  BouncingEntity(int x, int y, int bVel);
  ~BouncingEntity();

private:
  int x, y;
  int bVel;
  float bDelay;
  float lastBounce;
};
