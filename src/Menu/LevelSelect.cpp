#include "LevelSelect.h"


LevelSelect::LevelSelect() {
  // Load media
  for(int i = 0; i < TOTAL_SONGS; i++) {
    songSection[i].name = new LTexture();
    if (!songSection[i].name->loadFromRenderedText(gRenderer, gFont[NEXA_BOLD_10], SONG[i].name, WHITE)) 
      printf("Failed to load %s name text texture! SDL_Error: %s\n", SONG[i].name.c_str(), SDL_GetError());
    
    songSection[i].author = new LTexture();
    if (!songSection[i].author->loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], SONG[i].author, WHITE)) 
      printf("Failed to load %s author text texture! SDL_Error: %s\n", SONG[i].author.c_str(), SDL_GetError());
    }
}

LevelSelect::~LevelSelect() {

}

int LevelSelect::handleKeyPress(const SDL_Event e, Player* p) {
  p->handleKeyPress();
  // If player select the song
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_RETURN:
      return p->getPosY() / SECTION_HEIGHT;
      case SDLK_ESCAPE:
      return MENU;
    }
  }
  return -1;
}

void LevelSelect::render(Player *p) {
  int next_selected_song = p->getPosY() / SECTION_HEIGHT;
  if (next_selected_song != selected_song) {
    // Reload the music and selected_xong
    Mix_HaltMusic();
    Mix_PlayMusic(SONG[next_selected_song].preview_music, -1);
    selected_song = next_selected_song;
  }
  for (int i = 0; i < TOTAL_SONGS; i++) {
    SDL_Rect r = {0, SECTION_HEIGHT * i, SCREEN_WIDTH, SECTION_HEIGHT};
    SDL_Color col = (i == selected_song) ? SELECTED : UNSELECTED;
    SDL_SetRenderDrawColor(gRenderer, col.r, col.g, col.b, 0xFF);
    SDL_RenderFillRect(gRenderer, &r);
    // Render the song name
    LTexture *songName = songSection[i].name;
    songName->renderCenter(gRenderer, SCREEN_WIDTH - 40 - songName->getWidth(), i * SECTION_HEIGHT + 10 + songName->getHeight(), songName->getWidth(), songName->getHeight());
  }

  SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
  for(int i = 0; i < TOTAL_SONGS; i++) {
    SDL_RenderDrawLine(gRenderer, 0, i * SECTION_HEIGHT, SCREEN_WIDTH, i * SECTION_HEIGHT);
  }
}

void LevelSelect::refresh(Player *p) {
  p->refresh();
}
