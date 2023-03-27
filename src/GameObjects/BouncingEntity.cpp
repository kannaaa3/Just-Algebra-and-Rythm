#include "BouncingEntity.h"
#include <iostream>
using namespace std;

BouncingEntity::BouncingEntity(SDL_Rect r, int distance, float time)
    : Entity(r.x, r.y, r.w, r.h) {
  this->bTime = time;
  posY[UP] = y;
  posY[DOWN] = y + distance;
  dir = DOWN;
  bLast = -B_GAP;
  cout << "Init: y = " << posY[UP] << ".." << posY[DOWN] << endl;
}

BouncingEntity::~BouncingEntity() {}

void BouncingEntity::move() {
  if (gTimer.getTicks() >= bLast + B_GAP) {
    // bounce
    int shiftY = (float)(posY[DOWN] - posY[UP]) * 40.0 / bTime;
    y += (1 * (dir == DOWN) - 1 * (dir == UP)) * shiftY;

    bLast = gTimer.getTicks();
    if (y >= posY[DOWN])
      dir = UP;
    if (y <= posY[UP]) {
      dir = DOWN;
      bLast += B_DELAY;
    }
  }
}

BouncingText::BouncingText(string text, TTF_Font *font, int x, int y,
                           float startTime, float endTime,
                           float timePerBounce, SDL_Color color)
    : BouncingEntity({x, y, 0, 0}, 40, timePerBounce) {
  if (!textTexture.loadFromRenderedText(gRenderer, font, text, color)) {
    printf("Unable to render bouncing text texture!\n");
  } else {
    this->w = textTexture.getWidth();
    this->h = textTexture.getHeight();
    this->startTime = startTime;
    this->endTime = endTime;
    this->state = IDLE;
    this->renderQuad = {0, this->h / 2, this->w, 0};
    this->removable = false;
  }
}

BouncingText::~BouncingText(){};

void BouncingText::render() {
  if (gTimer.getTicks() < startTime)
    return;
  if (state == IDLE) {
    renderQuad.y -= 5;
    renderQuad.h += 10;
    if (renderQuad.y <= 0) {
      renderQuad.y = 0;
      renderQuad.h = this->h;
      state = NORMAL;
    }
  } else if (state == NORMAL) {
    move();
    if (gTimer.getTicks() >= endTime)
      state = DISSAPPEAR;
  } else {
    renderQuad.y += 8;
    renderQuad.h -= 16;
    if (renderQuad.h <= 0)
      removable = true;
  }
  textTexture.renderCenter(gRenderer, x, y, w, renderQuad.h, &renderQuad);
}
bool BouncingText::isRemovable() { return removable; }
