#include "MovingEntity.h"
#include <iostream>
using namespace std;

MovingEntity::MovingEntity(SDL_Rect r, MovingProperties m) {
  this->x = r.x; this->y = r.y;
  this->w = r.w; this->h = r.h;

  this->vel = m.vel; this->dir = m.dir;
  this->angle = m.angle; this->angleVel = m.angleVel;
}

MovingEntity::~MovingEntity() {

}

void MovingEntity::move() {
  // Spin the entity
  this->angle = fmod(this->angle + this->angleVel, 360);

  if (this->vel == 0) return;
  // Move the entity
  this->x +=  this->vel * cos(this->dir * M_PI / 180);
  this->y += -this->vel * sin(this->dir * M_PI / 180);
}

