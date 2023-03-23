#pragma once

#include "../constants.h"
#include "../util.h"
#include "Player.h"

// TODO: Add moving later
class SpinningRect {
public:
  enum SpinningRectState {
    IDLE = 0,
    SPLASH,
    NORMAL,
    MOVE,
    DISAPPEAR,
    TOTAL_STATES
  };

  const int MAX_ALPH = 150;
  const int MAX_SPLASH_EXTEND = 20;
  const int TIME_NORMAL = 40000;
  SpinningRect();
  ~SpinningRect();
  void loadMedia();

  // Check whether it collide
  bool checkCollision(Player *p);

  // Render from center
  void render(SDL_Point *center = NULL);
  void setPosition(int x, int y);
  void setDimension(int w, int h);
  void setHeight(int h);
  void setAngle(float angle);
  void setAngleVel(float angleVel);
  void setTimeNormal(float time);
  void setVel(int vel);
  void setDirection(float dir);

  void actByState();
  void idle();
  void splash();
  void normal();
  void move();
  void disappear();

  LTexture spinningRectTexture[TOTAL_STATES];
  SpinningRectState getState();

private:
  // Position of the center of rectangle
  int posX, posY;
  // Width of Rect. Should be even for prettiness. Or whatever you want.
  int width, height;
  float rAngle;
  float rAngleVel;
  int rAlpha;
  int rExtend;
  int rVel;
  float rDirection;
  float startNormal;
  float timeNormal;

  SpinningRectState rState;
  vector<int> numFrame = {10, 4, 1, 5};
};
