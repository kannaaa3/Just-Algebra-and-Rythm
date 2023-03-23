#include "SqrSnake.h"

SqrSnake::SqrSnake(SDL_Point p, float startTime, float dir, float vel,
                   int snakeSize, int snakeLen)
    : Enemy(MovingEntity({p.x, p.y, snakeSize, snakeSize}, {vel, dir, 0, 0}),
            {startTime, 0, 0, 40000 / vel}) {
  this->gapSqr = min(10, snakeSize / 7);
  this->s.len = snakeLen;
  this->s.size = snakeSize;
  this->lastSplash = 0;
}

SqrSnake::~SqrSnake() {}

void SqrSnake::loadMediaSnk() {
  this->mTexture[SPLASH].setDimension(s.size * 4 / 3, s.size * 4 / 3);
}

bool SqrSnake::checkCollision(Player *p) { return false; }

void SqrSnake::render() {
  for (float i = 0; i < s.len; i++) {
    pair<int, int> shift = shiftXY(-(gapSqr + s.size), shape.dir);
    mTexture[NORMAL].renderCenter(gRenderer, shape.x + i * shift.first,
                                  shape.y + i * shift.second, s.size, s.size);
  }
  if (gTimer.getTicks() - lastSplash >= splash_gap) {
    mTexture[SPLASH].renderCenter(gRenderer, shape.x, shape.y, s.size * 4 / 3,
                                  s.size * 4 / 3);

    pair<int, int> shift = shiftXY(-(gapSqr + s.size), shape.dir);
    SDL_Rect renderRect = {0, 0, s.size / 4 * 3, s.size / 4 * 3};
    mTexture[NORMAL].renderCenter(gRenderer, shape.x + s.len * shift.first,
                                  shape.y + s.len * shift.second,
                                  s.size / 4 * 3, s.size / 4 * 3, &renderRect);
    if (gTimer.getTicks() - lastSplash >= splash_gap + splash_delay)
      lastSplash = gTimer.getTicks();
  }
}
