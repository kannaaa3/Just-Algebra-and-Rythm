#include "LevelSelect.h"


LevelSelect::LevelSelect() {
  p = new Player(960, 540);

  // Load media
  for(int i = 0; i < TOTAL_SONGS; i++) {
    songSection[i].name = new LTexture();
    if (!songSection[i].name->loadFromRenderedText(gRenderer, gFont[NEXA_BOLD_10], SONG[i].name, WHITE)) 
      printf("Failed to load %s name text texture! SDL_Error: %s\n", SONG[i].name.c_str(), SDL_GetError());
    
    songSection[i].author = new LTexture();
    if (!songSection[i].author->loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], SONG[i].author, WHITE)) 
      printf("Failed to load %s author text texture! SDL_Error: %s\n", SONG[i].author.c_str(), SDL_GetError());

    songSection[i].music = Mix_LoadMUS(("assets/global/sound/" + SONG[i].file_preview).c_str());
    if (NULL == songSection[i].music) 
      printf("Failed to load beat music: %s! SDL_mixer Error: %s\n", SONG[i].file_preview.c_str(), Mix_GetError());
  }
}

LevelSelect::~LevelSelect() {
}

void LevelSelect::handleKeyPress(const SDL_Event e) {

}

void LevelSelect::render(Player *p) {
  int next_selected_song = p->getPosY() / SECTION_HEIGHT;
  if (next_selected_song != selected_song) {
    // Reload the music and selected_xong
  }

  for (int i = 0; i < TOTAL_SONGS; i++) {
    if (i == selected_song) {
      // render some how

    } else {
      // render some how

    }

  }
}

void LevelSelect::refresh() {

}
