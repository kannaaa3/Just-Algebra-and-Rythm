#include "Setting.h"

Setting::Setting() {
  spinSqr = new LTexture();

  if (!spinSqr->loadFromFile(gRenderer, "assets/GameObjects/img/PlayerIdle.png")) {
    printf("Failed to load spinSqr! SDL_Error: %s", SDL_GetError());
  } else {
    spinSqr->setDimension(40, 40);
  }

  for(int i = TITLE; i != TOTAL_TEXT; i++) {
    for (int j = 0; j != TOTAL_COL; j++) {
      textTexture[i][j] = new LTexture();
    }
  }
  
  const vector<string> text = {"SETTINGS", "music", "sound fx"};
  const vector<int> fontID = {NEXA_BOLD_10, NEXA_LIGHT_10, NEXA_LIGHT_10};
  for(int i = 0; i != TOTAL_TEXT; i++) {
    if (!textTexture[i][WHITE_ID]->loadFromRenderedText(gRenderer, gFont[fontID[i]], text[i], WHITE) ||
        !textTexture[i][BLUE_ID]->loadFromRenderedText(gRenderer, gFont[fontID[i]], text[i], ALLY_BLUE)) {
      printf("Failed to create %s texture! SDL_Error: %s\n", text[i].c_str(), SDL_GetError());
    }
  }

  selected_section = MUSIC;
  vol[MUSIC] = vol[SOUND_FX] = 127;

  sfxNavigate = Mix_LoadWAV("assets/global/sound/setting_navigate.wav");
  sfxAdjust = Mix_LoadWAV("assets/global/sound/setting_sfx.wav");
  if(sfxAdjust == NULL || sfxNavigate == NULL) {
    printf( "Failed to load SFX! SDL_mixer Error: %s\n", Mix_GetError());
  }
}

Setting::~Setting() {
  for(int i = TITLE; i != TOTAL_TEXT; i++) 
    for(int j = 0; j!= TOTAL_COL; j++)
      textTexture[i][j]->free();
  spinSqr->free();

  Mix_FreeChunk(sfxNavigate);
  Mix_FreeChunk(sfxAdjust);
}

bool Setting::handleKeyPress(const SDL_Event e) {
  bool quit = false;
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_UP:
      if (selected_section > MUSIC) selected_section--;
      Mix_PlayChannel(-1, sfxNavigate, 0);
      break;
      case SDLK_DOWN:
      if (selected_section < SOUND_FX) selected_section++;
      Mix_PlayChannel(-1, sfxNavigate, 0);
      break;
      case SDLK_LEFT: {
      vol[selected_section] = max (0, vol[selected_section] - OFFSET);
      Mix_PlayChannel(-1, sfxAdjust, 0);
      break;
      }
      case SDLK_RIGHT: {
      vol[selected_section] = min (127, vol[selected_section] + OFFSET);
      Mix_PlayChannel(-1, sfxAdjust, 0);
      break;
      }
      case SDLK_ESCAPE:
      //TODO: HELP!!
      quit = true;
      break;
    }
    if (selected_section == MUSIC) Mix_VolumeMusic(vol[MUSIC]);
    else Mix_Volume(-1, vol[SOUND_FX]);
  }
  return quit;
}

void Setting::render() {
  // Black BG other than blue
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(gRenderer);
  // Top rectangle: OPTIONS
  SDL_SetRenderDrawColor(gRenderer, 9, 146, 156, 0xFF);
  SDL_Rect r = {0, 0, 1920, 80};
  SDL_RenderFillRect(gRenderer, &r);

  textTexture[TITLE][WHITE_ID]->renderCenter(gRenderer, pos[TITLE].x, pos[TITLE].y, textTexture[TITLE][WHITE_ID]->getWidth(), textTexture[TITLE][WHITE_ID]->getHeight());

  for(int i = MUSIC; i != TOTAL_TEXT; i++) {
    if (selected_section != i)
      textTexture[i][WHITE_ID]->render(gRenderer, pos[i].x, pos[i].y);
    else 
      textTexture[i][BLUE_ID]->render(gRenderer, pos[i].x, pos[i].y);
    int numVol = vol[i] * 50 / 127;
    int x = 500, y = pos[i].y, SIZE = 20;
    for (int i = 0; i < numVol; i+=1) {
      SDL_SetRenderDrawColor(gRenderer, ENEMY_PINK.r, ENEMY_PINK.g, ENEMY_PINK.b, 0xFF);
      SDL_Rect tmpRect = {x + (SIZE + 2) * i, y, SIZE, SIZE};
      SDL_RenderFillRect(gRenderer, &tmpRect);
    }
    SDL_SetRenderDrawColor(gRenderer, WHITE.r, WHITE.g, WHITE.b, 0xFF);
    SDL_Rect tmpRect = {x + (SIZE + 2) * max(0, (numVol - 1)), y, SIZE, SIZE};
    SDL_RenderFillRect(gRenderer, &tmpRect);

  }

  // Render Spin Square
  angle = fmod(angle - 4, 360);
  spinSqr->renderCenter(gRenderer, pos[selected_section].x - 50, pos[selected_section].y + 15, 40, 40, NULL, angle);
}

void Setting::refresh() {
  selected_section = MUSIC;
}
