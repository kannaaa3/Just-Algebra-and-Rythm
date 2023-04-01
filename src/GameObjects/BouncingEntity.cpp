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

BouncingText::BouncingText(string text, int fontType, int x, int y,
                           float startTime, float endTime, float timePerBounce,
                           SDL_Color color)
    : BouncingEntity({x, y, 0, 0}, 40, timePerBounce) {
  // NOTE: If not, seg fault
  textTexture = new LTexture();
  if (!this->textTexture->loadFromRenderedText(gRenderer, gFont[fontType],
                                        text, color)) {
    printf("Unable to render bouncing text texture!\n");
  } else {
    cout << "Successfully created a new Bouncing Text." << endl;

    this->w = textTexture->getWidth();
    this->h = textTexture->getHeight();
    this->startTime = startTime;
    this->endTime = endTime;
    this->state = IDLE;
    this->renderQuad = {0, this->h / 2, this->w, 0};
    this->removable = false;
    
      // cout <<" LTExture " << &this->textTexture << endl;
  }
}

BouncingText::~BouncingText(){};

void BouncingText::render() {
  // cout << "2" << endl;
      // cout <<"LTexture " << &textTexture << endl;

  if (gTimer.getTicks() < startTime) {
    cout << "Text not start yet!" << endl;
    return;
  }
  // If pause, not moving or changing state
  if (not gTimer.isPaused()) {
  switch (state) {
  case IDLE: {
    renderQuad.y -= 5;
    renderQuad.h += 10;
    if (renderQuad.y <= 0) {
      renderQuad.y = 0;
      renderQuad.h = this->h;
      state = NORMAL;
    }
    break;
  }
  case NORMAL: {
    move();
    if (gTimer.getTicks() >= endTime)
      state = DISSAPPEAR;
    break;
  }
  case DISSAPPEAR: {
    renderQuad.y += 8;
    renderQuad.h -= 16;
    if (renderQuad.h <= 0)
      removable = true;
    break;
  }
  }
  }

  // cout << "Renderer : " <<  gRenderer << endl;
  textTexture->renderCenter(gRenderer, x, y, w, 
                           renderQuad.h, &renderQuad);
  // SDL_RenderPresent(gRenderer);
}

bool BouncingText::isRemovable() { return removable; }
float BouncingText::getStartTime() { return startTime; }

void BouncingText::loadMedia() {}
