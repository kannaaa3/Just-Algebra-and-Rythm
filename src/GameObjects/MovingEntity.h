#pragma once
#include "../constants.h"
#include "Entity.h"

// Moving Entity is NOT Renderable!
// It only help control the movement of entity.
class MovingEntity : public Entity {
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

  float vel, dir;
  float angle, angleVel;

  void move();

private:
};
