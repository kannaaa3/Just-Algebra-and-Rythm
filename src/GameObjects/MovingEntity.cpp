#include "MovingEntity.h"

MovingEntity::MovingEntity(SDL_Rect r, MovingProperties m) {
  this->x = r.x;
  this->y = r.y;
  this->w = r.w;
  this->h = r.h;

  this->vel = m.vel;
  this->dir = m.dir;
  this->angle = m.angle;
  this->angleVel = m.angleVel;
}

MovingEntity::~MovingEntity() {}

void MovingEntity::move() {
  // Spin the entity
  angle = fmod(angle + angleVel, 360);

  if (vel == 0)
    return;
  // Move the entity
  x += vel * cos(dir * M_PI / 180);
  y += -vel * sin(dir * M_PI / 180);
}
