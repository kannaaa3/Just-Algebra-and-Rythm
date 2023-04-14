#pragma once

#include "../util.h"
#include "../constants.h"

class TransitionEffect {
public:
  // If state is true (active), transition. Otherwise, no transition.
  TransitionEffect(bool state = false);
  ~TransitionEffect();

  void render();
  void fade();
  bool isFading();

  void refresh();

  bool isRemovable();

private:
  const int W = 226, H = 300, SQR_SIZE = 160, STEP = 3;
  const static int TOTAL_ROW = 8, TOTAL_COL = 12, TOTAL_SPRITE = 7, NUM_FRAME = 2;
  int lastRow, alpha, lastRowSpr;

  LTexture *transSqrSprites, *whiteSqr;
  SDL_Rect renderQuad[TOTAL_SPRITE];

};

