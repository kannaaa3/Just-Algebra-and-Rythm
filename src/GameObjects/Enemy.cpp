#include "Enemy.h"
#include <iostream>
using namespace std;

Enemy::Enemy(MovingEntity me, EnemyTimer t, int alpha, int splash_extend,
             State s) {
  // Entity's properties
  this->shape = me;
  this->time = t;

  mAlpha = alpha;
  mSplashExtend = splash_extend;

  mState = s;

  removable = false;
}

Enemy::~Enemy() {
  for (int i = 0; i != TOTAL_STATES; i++) {
    mTexture[i].free();
  }
}

void Enemy::loadMedia() {
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
}

void Enemy::actByState() {
  switch (mState) {
  case IDLE:
    idle();
    break;
  case SPLASH:
    splash();
    break;
  case NORMAL:
    normal();
    break;
  case DISAPPEAR:
    disappear();
    break;
  }
}

void Enemy::idle() {
  if (gTimer.getTicks() >= this->time.start + this->time.idle) {
    // Set maximum opacity
    mAlpha = 255;
    mState = SPLASH;
    // Update start time
    this->time.start = gTimer.getTicks();
    numIncAlph = 1; // 1ms
  } else {
    // Increase alpha
    if (mAlpha < MAX_IDLE_ALPHA)
      mAlpha += 2;
  }
}

void Enemy::splash() {
  if (gTimer.getTicks() >= time.start + time.splash) {
    mAlpha = 255;
    mState = NORMAL;
    mSplashExtend = 0;
    time.start = gTimer.getTicks();
  } else {
    if (gTimer.getTicks() >= time.start + numIncAlph * 50) {
      // TODO: Do something with this alph of splash
      int curAlph = 255 - (100 * 50 * numIncAlph / time.splash);
      mAlpha = curAlph;
      numIncAlph++;
    }

    if (mSplashExtend > 0)
      mSplashExtend--;
    mTexture[SPLASH].setDimension(shape.w + mSplashExtend,
                                  shape.h + mSplashExtend);
  }
}

void Enemy::normal() {
  if (gTimer.getTicks() >= time.start + time.normal) {
    mState = DISAPPEAR;
    time.start = gTimer.getTicks();
  } else {
    // Move the entity
    shape.move();
  }
}

void Enemy::disappear() {
  if (shape.h <= 0 || mAlpha < 10) {
    // It's time to remove this
    mAlpha = 0;
    removable = true;
  } else {
    // Decrease opacity until 0
    shape.h -= 3;
    mTexture[DISAPPEAR].setDimension(shape.w, shape.h);
    mAlpha -= 10;
  }
}

void Enemy::render() {
  // cout << "DEBUG: mState = " << mState << ", mAlpha = " << mAlpha << endl;
  actByState();

  if (mState == SPLASH) {
    mTexture[NORMAL].renderCenter(gRenderer, shape.x, shape.y, shape.w, shape.h,
                                  NULL, shape.angle);
  }

  mTexture[mState].setAlpha(mAlpha);
  mTexture[mState].renderCenter(
      gRenderer, shape.x, shape.y, shape.w + (SPLASH == mState) * mSplashExtend,
      shape.h + (SPLASH == mState) * mSplashExtend, NULL, shape.angle);
}

bool Enemy::checkCollision(Player *p) {
  SDL_FRect enemy = {(float)shape.x, (float)shape.y, (float)shape.w,
                     (float)shape.h};
  SDL_FRect pRect = {(float)p->getPosX(), (float)p->getPosY(), (float)p->P_SIZE,
                     (float)p->P_SIZE};
  return checkCollisionRotate(enemy, pRect, shape.angle) |
         checkCollisionRotate(pRect, enemy, 0);
}

bool Enemy::isRemovable() { return removable; }

// Setter and Getter
void Enemy::setState(State s) { mState = s; }
void Enemy::setStartTime(float t) { time.start = t; }

Enemy::State Enemy::getState() { return mState; }
float Enemy::getStartTime() { return time.start; }
