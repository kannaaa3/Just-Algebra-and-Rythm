#pragma once
#include "../constants.h"

// Moving Entity is NOT Renderable!
// It only help control the movement of entity.
class MovingEntity {
public:
  struct MovingProperties {
    float vel;
    float dir;
    float angle;
    float angleVel;
  };

  MovingEntity(SDL_Rect r = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0},
               MovingProperties m = {0, 0, 0, 0});
  ~MovingEntity();

  // CENTER position and shape of the entity on the screen
  // By default, this lies on the center of screen, dimension of entity is zero.
  // x = SCREEN_WIDTH / 2; x = SCREEN_HEIGHT/2, w = 0, h = 0
  int x, y;
  int w, h;
  // By default, the entity does not spin nor move.
  float vel, dir;
  float angle, angleVel;

  void move();

private:
};
