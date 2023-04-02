#include "Transition.h"

TransitionEffect::TransitionEffect(bool state) {
  transSqrSprites = new LTexture();
  whiteSqr = new LTexture();
  alpha = (state) ? 0xFF : 0;
  lastRowSpr = 0;
  lastRow = 0;
  if (!transSqrSprites->loadFromFile(gRenderer, "assets/global/img/TransitionSquareSprites.png")) {
    printf("Failed to load TransitionSquareSprites texture!\n");
  } else {
    for (int i = 0; i < TOTAL_SPRITE; i++) {
      renderQuad[i] = {W * i, 0, W, H};
    }
  }
  if (!whiteSqr->loadFromFile(gRenderer, "assets/global/img/whiteBG.png")) {
    printf("Failed to load whiteBG texture!\n");
  }
}

TransitionEffect::~TransitionEffect() {
  transSqrSprites->free();
  whiteSqr->free();
}

void TransitionEffect::fade() {
  alpha = max (0, alpha - 15);
  // Render a white screen
  SDL_Rect r = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  // FIXME: Didnt work
  // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0);
  // SDL_RenderFillRect(gRenderer, &r);
  whiteSqr->setAlpha(alpha);
  whiteSqr->render(gRenderer, 0, 0);

}

bool TransitionEffect::isFading() {
  return 0 <= alpha && alpha <= 0xFF && TOTAL_ROW == lastRow;
}

void TransitionEffect::render() {
  if (lastRow == TOTAL_ROW) {
    // fade();
    return;
  } 

  if (lastRow >= 2) {
    // White rectangle 
    SDL_Rect r = {0, 0, 1920, SQR_SIZE * (lastRow - 1)};
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &r);
  }
  for (int colNum = 0; colNum < TOTAL_COL; colNum++) {
    int x_center = SQR_SIZE/2 + colNum * SQR_SIZE;
    int y_center = SQR_SIZE/2 + lastRow * SQR_SIZE;
    transSqrSprites->renderCenter(gRenderer, x_center, y_center, W, H, &renderQuad[(lastRowSpr)/NUM_FRAME]);
    if (lastRow >= 1) 
      transSqrSprites->renderCenter(gRenderer, x_center, y_center - SQR_SIZE, W, H, &renderQuad[lastRowSpr/NUM_FRAME + STEP]);
  }
  // Move next sprite
  lastRowSpr ++;
  if (lastRowSpr  >= 3 * NUM_FRAME) {
    lastRow++;
    lastRowSpr = 0;
  }
}

void TransitionEffect::refresh() {
  alpha = 0xFF;
  lastRowSpr = 0;
  lastRow = 0;
}

bool TransitionEffect::isRemovable() { return alpha == 0; }
