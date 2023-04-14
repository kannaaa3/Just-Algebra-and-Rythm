#include "GameOver.h"

GameOver::GameOver() {
  for(int i = 0; i < TOTAL_TEXT; i++) {
    text[i] = new LTexture();
    if (i < 2) selectedText[i] = new LTexture();
  }
  selectEffect = new LTexture();
  background = new LTexture();

  const string txt[] = {"Yes", "No", "Try Again?", "IT'S NOT OVER"};
  if (!text[GAMEOVER]->loadFromRenderedText(gRenderer, gFont[NEXA_BOLD_20], txt[GAMEOVER], ALLY_BLUE)) {
    printf("Failed to load %s texture! SDL_Error: %s\n", txt[GAMEOVER].c_str(), SDL_GetError());
  }
  for(int i = YES; i != GAMEOVER; i++) {
    if (!text[i]->loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], txt[i], WHITE)) {
    printf("Failed to load %s texture! SDL_Error: %s\n", txt[i].c_str(), SDL_GetError());
    }
  }
  for(int i = 0; i < 2; i ++) {
    if (!selectedText[i]->loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], txt[i], ALLY_BLUE)) {
      printf("Failed to load %s texture! SDL_Error: %s\n", txt[i].c_str(), SDL_GetError());
    }
  }
  if (!selectEffect->loadFromFile(gRenderer, "assets/global/img/selectedTextEffect.png")) {
      printf("Failed to load selectedTextEffect texture! SDL_Error: %s\n", SDL_GetError());
  }
  if (!background->loadFromFile(gRenderer, "assets/global/img/GameOverBG.png")) {
      printf("Failed to load Game over BG texture! SDL_Error: %s\n", SDL_GetError());
  } else background->setDimension(1920, 1080);
  selectedSection = 0;
}

GameOver::~GameOver() {
  for(int i = 0; i < TOTAL_TEXT; i++) {
    text[i]->free();
    if (i < 2) selectedText[i]->free();
  }
}

void GameOver::refresh() {
  selectedSection = 0;
  gMusic = Mix_LoadMUS(("assets/global/sound/waterfall.wav"));
  if (gMusic == NULL) {
    printf("Failed to load Game Over BGM music! SDL_mixer Error: %s\n", Mix_GetError());
  }
  Mix_PlayMusic(gMusic, -1);
}

int GameOver::handleKeyPress(const SDL_Event e) {
  //TODO: DELETE
  int ingame = 0;
  int gameOva = 5;
  int levelSelect = 6;
  // cout << "GAMEOVER: handleKeyPress" << endl;
  if (SDL_KEYDOWN == e.type) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT: {
      if (selectedSection > YES) selectedSection--;
      break;
      }
      case SDLK_RIGHT: {
      if (selectedSection < NO) selectedSection++;
      break;
      }
      case SDLK_RETURN: {
        if (selectedSection == YES) {
          return ingame;
        } else {
          return levelSelect;
        }
      break;
      }
    }
  }
  return gameOva;
}

void GameOver::render() {
  background->render(gRenderer, 0, 0);
  for(int i = 0; i < TOTAL_TEXT; i++) {
    if (selectedSection == i) {
      selectedText[i]->renderCenter(gRenderer, pos[i].x, pos[i].y, text[i]->getWidth(), text[i]->getHeight());
      selectEffect->renderCenter(gRenderer, pos[i].x, pos[i].y, selectEffect->getWidth(), selectEffect->getHeight());
    }
   else  {
      text[i]->renderCenter(gRenderer, pos[i].x, pos[i].y, text[i]->getWidth(), text[i]->getHeight());
    }
  }
  
}

