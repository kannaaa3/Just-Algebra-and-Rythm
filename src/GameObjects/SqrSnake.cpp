#include "SqrSnake.h"

SqrSnake::SqrSnake(float startTime, SDL_Point p, float dir, float vel,
                   int snakeSize, int snakeLen)
    : Enemy(MovingEntity({p.x, p.y, snakeSize, snakeSize}, {vel, dir, 0, 0}),
            {startTime, 0, 0, 40000 / vel}) {
  this->gapSqr = min(10, snakeSize / 7);
  this->s.len = snakeLen;
  this->s.size = snakeSize;
  this->lastSplash = 0;
}

SqrSnake::~SqrSnake() {}

void SqrSnake::loadMedia() {
  string prefix = "assets/Enemy/";

  if (!mTexture[IDLE].loadFromFile(gRenderer, prefix + "FilledRect.png"))
    printf("Failed to load FilledRectIDLE texture!\n");

  if (!mTexture[NORMAL].loadFromFile(gRenderer, prefix + "FilledRect.png"))
    printf("Failed to load FilledRect texture!\n");

  if (!mTexture[SPLASH].loadFromFile(gRenderer,
                                     prefix + "FilledRectSplash.png"))
    printf("Failed to load FilledRectSplash texture!\n");

  if (!mTexture[DISAPPEAR].loadFromFile(gRenderer, prefix + "FilledRect.png"))
    printf("Failed to load FilledRect texture!\n");

  for (int i = IDLE; i != TOTAL_STATES; i++) {
    mTexture[i].setDimension(this->shape.w, this->shape.h);
  }

  this->mTexture[SPLASH].setDimension(s.size * 4 / 3, s.size * 4 / 3);
}

bool SqrSnake::checkCollision(Player *p) {
  if (p->isInvincible()) return false;
  SDL_FRect pRect = {(float)p->getPosX(), (float)p->getPosY(), (float)p->P_SIZE-5,
                     (float)p->P_SIZE-5};
  SDL_FRect enemy = {
      (float)shape.x - (- s.size / 2 + s.len * (gapSqr + s.size) / 2) * cos(toRad(shape.dir)),
      (float)shape.y + (- s.size / 2 + s.len * (gapSqr + s.size) / 2) * sin(toRad(shape.dir)), (float)s.len * (gapSqr + s.size), (float)shape.h};
  return (checkCollisionRotate(enemy, pRect, 0) ||
          checkCollisionRotate(pRect, enemy, 0));
}

void SqrSnake::render() {
  actByState();
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
