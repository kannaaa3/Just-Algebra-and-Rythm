#include "MovingEntity.h"
#include <iostream>
using namespace std;

MovingEntity::MovingEntity(int x, int y, int w, int h,
                           float angle, float angleVel, int vel, float dir,
                           float idle_time, float splash_time, float normal_time,
                           int alpha, int splash_extend, State s) {
  // Entity's dimension
  mPosX = x; mPosY = y;
  mWidth = w; mHeight = h;

  mAngle = angle; mAngleVel = angleVel;
  mVel = vel; mDir = dir;

  normalTime = normal_time;
  splashTime = splash_time;
  idleTime = idle_time;

  mAlpha = alpha;
  mSplashExtend = splash_extend;

  mState = s;

  removable = false;
}

MovingEntity::~MovingEntity() {
  for (int i = 0; i != TOTAL_STATES; i++) {
    mTexture[i].free();
  }
}

void MovingEntity::loadMedia() {
  string prefix = "../assets/Enemy/";

  if (!mTexture[IDLE].loadFromFile(gRenderer, prefix + "FilledRect.png")) 
    printf("Failed to load FilledRectIDLE texture!\n");

  if (!mTexture[NORMAL].loadFromFile(gRenderer, prefix + "FilledRect.png")) 
    printf("Failed to load FilledRect texture!\n");
  
  if (!mTexture[SPLASH].loadFromFile(gRenderer, prefix + "FilledRectSplash.png")) 
    printf("Failed to load FilledRectSplash texture!\n");

  if (!mTexture[DISAPPEAR].loadFromFile(gRenderer, prefix + "FilledRect.png")) 
    printf("Failed to load FilledRect texture!\n");

  for (int i = IDLE; i != TOTAL_STATES; i++) {
    mTexture[i].setDimension(mWidth, mHeight);
  }

}

void MovingEntity::actByState() {
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

void MovingEntity::idle () {
  if (gTimer.getTicks() >= startTime + idleTime) {
    // Set maximum opacity
    mAlpha = 255; 
    mState = SPLASH;
    // Update start time
    startTime = gTimer.getTicks();
    numIncAlph = 1; // 1ms
  } else {
    // Increase alpha
    if (mAlpha < MAX_IDLE_ALPHA)
      mAlpha += 2;
  }
}

void MovingEntity::splash() {
  if (gTimer.getTicks() >= startTime + splashTime) {
    mAlpha = 255;
    mState = NORMAL;
    mSplashExtend = 0;
    startTime = gTimer.getTicks();
  } else {
    if (gTimer.getTicks() >= startTime + numIncAlph * 50) {
      // TODO: Do something with this alph of splash
      int curAlph = 255 - (100 * 50 * numIncAlph / splashTime);
      mAlpha = curAlph;
      numIncAlph ++;
    }

    if (mSplashExtend > 0) mSplashExtend --;
    mTexture[SPLASH].setDimension(mWidth + mSplashExtend, mHeight + mSplashExtend);
  }
}

void MovingEntity::normal() {
  if (gTimer.getTicks() >= startTime + normalTime) {
    mState = DISAPPEAR;
    startTime = gTimer.getTicks();
  } else {
    // Move the entity
    mAngle = fmod(mAngle + mAngleVel, 360);
    if (mVel == 0) return;
    int shiftX =  mVel * cos(mDir * M_PI / 180);
    int shiftY = -mVel * sin(mDir * M_PI / 180);
    mPosX += shiftX;
    mPosY += shiftY;
  }
}

void MovingEntity::disappear() {
  if (mHeight <= 0 || mAlpha < 10) {
    // It's time to remove this
    mAlpha = 0;
    removable = true;
  } else {
    // Decrease opacity until 0
    mHeight -= 3;
    mTexture[DISAPPEAR].setDimension(mWidth, mHeight);
    mAlpha -= 10;
  }
}

void MovingEntity::render() {
  // cout << "DEBUG: mState = " << mState << ", mAlpha = " << mAlpha << endl;

  if (mState == SPLASH) {
    mTexture[NORMAL].renderCenter(gRenderer, mPosX, mPosY, mWidth, mHeight, NULL, mAngle);
  }

  mTexture[mState].setAlpha(mAlpha);
  // FIXME: Whenever this func runs, player do not splash!!!! If remove, player splash again
  mTexture[mState].renderCenter(gRenderer, mPosX, mPosY, mWidth + (SPLASH == mState) * mSplashExtend, mHeight + (SPLASH == mState) * mSplashExtend, NULL, mAngle);
}

bool MovingEntity::isRemovable() {return removable;}

// Setter and Getter
void MovingEntity::setPosition(int x, int y) { mPosX = x; mPosY = y; }
void MovingEntity::setDimension(int w, int h) { mWidth = w; mHeight = h; }
void MovingEntity::setAngle(float angle) { mAngleVel = angle; }
void MovingEntity::setAngleVel(float angleVel) { mAngleVel = angleVel; }
void MovingEntity::setVel(int vel) { mVel = vel; }
void MovingEntity::setDirection(float dir) { mDir = dir; }
void MovingEntity::setState(State s) { mState = s; }
void MovingEntity::setIdleTime(float t) { idleTime = t; }
void MovingEntity::setSplashTime(float t) { splashTime = t; }
void MovingEntity::setNormalTime(float t) { normalTime = t; }
void MovingEntity::setStartTime(float t) {startTime = t;}

pair<int, int> MovingEntity::getPosition() { return {mPosX, mPosY}; }
pair<int, int> MovingEntity::getDimension() { return {mWidth, mHeight}; }
float MovingEntity::getAngle() {return mAngle;}
float MovingEntity::getAngleVel() {return mAngleVel;}
int MovingEntity::getVel() {return mVel;}
float MovingEntity::getDirection() {return mDir;}
MovingEntity::State MovingEntity::getState() {return mState;}

