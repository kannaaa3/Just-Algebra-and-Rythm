#include "SpinningRect.h"
#include <iostream>
using namespace std;

SpinningRect::SpinningRect() {
  // THe Rect lies in center position by default
  posX = SCREEN_WIDTH / 2;
  posY = SCREEN_HEIGHT / 2;

  width = SCREEN_DIAGONAL;
  height = 30;
  rAngle = 0;
  rAngleVel = 0.5;
  rState = IDLE;
  rAlpha = 0;
  rExtend = 0;
  rVel = 0;
  rDirection = 0;
  startNormal = gTimer.getTicks();
  timeNormal = TIME_NORMAL;
}

void SpinningRect::loadMedia() {
  string prefix = "assets/Enemy/";

  if (!spinningRectTexture[IDLE].loadFromFile(gRenderer,
                                              prefix + "FilledRect.png"))
    printf("Failed to load FilledRectIDLE texture!\n");

  if (!spinningRectTexture[NORMAL].loadFromFile(gRenderer,
                                                prefix + "FilledRect.png"))
    printf("Failed to load FilledRect texture!\n");

  if (!spinningRectTexture[SPLASH].loadFromFile(
          gRenderer, prefix + "FilledRectSplash.png"))
    printf("Failed to load FilledRectSplash texture!\n");

  if (!spinningRectTexture[DISAPPEAR].loadFromFile(gRenderer,
                                                   prefix + "FilledRect.png"))
    printf("Failed to load FilledRect texture!\n");

  for (int i = IDLE; i != TOTAL_STATES; i++) {
    spinningRectTexture[i].setDimension(width, height);
  }
}

// TODO: Unit testing require. 4 corner of each respect to the other one
bool SpinningRect::checkCollision(Player *p) {
  // Glad I discover SDL_Frect!
  SDL_FRect rotatedRect;
  SDL_FPoint upperLeft;
  pair<float, float> topLeft =
      rotateAxis(rAngle, posX - width / 2, -(posY - height / 2));
  pair<float, float> botRight =
      rotateAxis(rAngle, posX + width / 2, -(posY + height / 2));

  rotatedRect.x = topLeft.first;
  rotatedRect.y = topLeft.second;
  rotatedRect.w = width;
  rotatedRect.h = height;
  // rotatedRect.w = botRight.first - topLeft.first;
  // rotatedRect.h = topLeft.second - botRight.second;
  // cout << "O " << topLeft.first <<", " << topLeft.second << endl;

  // Player's lower left and upper right corner coordinates respect to (0,0)
  SDL_FPoint pLowerLeft;
  SDL_FPoint pUpperRight;

  pair<float, float> pLL =
      rotateAxis(rAngle,
                 (float)(p->getPosX()) - (float)p->P_SIZE * 0.5 -
                     (float)SCREEN_WIDTH * 0.5,
                 (-1) * ((float)(p->getPosY()) + (float)p->P_SIZE * 0.5 -
                         (float)SCREEN_HEIGHT * 0.5));
  pair<float, float> pUR =
      rotateAxis(rAngle,
                 (float)(p->getPosX()) + (float)p->P_SIZE * 0.5 -
                     (float)SCREEN_WIDTH * 0.5,
                 (-1) * ((float)(p->getPosY()) - (float)p->P_SIZE * 0.5 -
                         (float)SCREEN_HEIGHT * 0.5));
  pLowerLeft.x = pLL.first;
  pLowerLeft.y = pLL.second;
  pUpperRight.x = pUR.first;
  pUpperRight.y = pUR.second;

  // printf("DEBUG: Check Collision:\n");
  // printf("Rect: x = %.3f, y = %.3f, w = %.3f, h = %.3f\n", rotatedRect.x,
  // rotatedRect.y, rotatedRect.w, rotatedRect.h);

  // printf("LowerLeft: x = %.3f, y = %.3f; UpperRight: x = %.3f, y = %.3f\n",
  // pLowerLeft.x, pLL.second, pUR.first, pUR.second);

  return false;

  return SDL_PointInFRect(&pLowerLeft, &rotatedRect) |
         SDL_PointInFRect(&pUpperRight, &rotatedRect);
}

void SpinningRect::actByState() {
  switch (rState) {
  case IDLE:
    idle();
    break;
  case SPLASH:
    splash();
    break;
  case NORMAL:
    normal();
    break;
  case MOVE:
    move();
    break;
  case DISAPPEAR:
    disappear();
    break;
  }
}

void SpinningRect::idle() {
  if (rAlpha == MAX_ALPH) {
    rState = SPLASH;
    rExtend = MAX_SPLASH_EXTEND;
    rAlpha = 255;
  } else {
    rAlpha++;
  }
}

void SpinningRect::splash() {
  if (rExtend == 0) {
    rState = NORMAL;
    rAlpha = 255;
  } else {
    spinningRectTexture[SPLASH].setDimension(width + rExtend, height + rExtend);
    rExtend--;
    rAlpha -= 20;
  }
}

void SpinningRect::normal() {
  if (gTimer.getTicks() - startNormal >= timeNormal) {
    rState = DISAPPEAR;
  } else {
    rAngle = fmod(rAngle + rAngleVel, 360);
    if (rVel != 0)
      move();
  }
}

void SpinningRect::move() {
  int shiftX = rVel * cos(rDirection / 180 * M_PI);
  int shiftY = -rVel * sin(rDirection / 180 * M_PI);

  posX += shiftX;
  posY += shiftY;
}

void SpinningRect::disappear() {
  if (rAlpha == 0) {
    // TODO: What? Stop pls
  }
  if (rAlpha >= 20) {
    rAlpha -= 20;
  } else
    rAlpha = 0;
}

void SpinningRect::render(SDL_Point *center) {
  // FIX: PROBLEM: have to set Dimension

  spinningRectTexture[rState].setAlpha(rAlpha);
  spinningRectTexture[rState].renderCenter(gRenderer, posX, posY, width, height,
                                           NULL, rAngle, center);
}

void SpinningRect::setPosition(int x, int y) {
  posX = x;
  posY = y;
}
void SpinningRect::setDimension(int w, int h) {
  width = w;
  height = h;
}
void SpinningRect::setHeight(int h) { height = h; }
void SpinningRect::setAngle(float angle) { rAngle = angle; }
void SpinningRect::setAngleVel(float angleVel) { rAngleVel = angleVel; }
void SpinningRect::setTimeNormal(float time) { timeNormal = time; }
void SpinningRect::setVel(int vel) { rVel = vel; }
void SpinningRect::setDirection(float dir) { rDirection = dir; }

SpinningRect::SpinningRectState SpinningRect::getState() { return rState; }

SpinningRect::~SpinningRect() {}
