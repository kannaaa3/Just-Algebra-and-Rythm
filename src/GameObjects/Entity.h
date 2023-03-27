#pragma once
#include "../constants.h"
#include "../util.h"

class Entity {
public:
  Entity(int x = SCREEN_WIDTH/2, int y = SCREEN_HEIGHT/2, int w = 0, int h = 0);
  ~Entity();
  // CENTER position and shape of the entity on the screen
  // By default, this lies on the center of screen, dimension of entity is zero.
  // x = SCREEN_WIDTH / 2; x = SCREEN_HEIGHT/2, w = 0, h = 0
  // By default, the entity does not spin nor move.
  int x, y;
  int w, h;
private:
};
