#pragma once

#include "../util.h"
#include "../GameObjects/Player.h"

class LevelSelect {
public:
  LevelSelect();
  ~LevelSelect();

  int handleKeyPress(const SDL_Event e, Player *p);
  void render(Player *p);
  void refresh(Player *p);

private:
  const int SECTION_HEIGHT = SCREEN_HEIGHT/ TOTAL_SONGS;
  const SDL_Color UNSELECTED = {3, 85, 93},
                  SELECTED   = {9, 146, 156};

  struct SongSection {
    LTexture *name, *author;
  } songSection[TOTAL_SONGS];

  // based on y coordinate
  int selected_song;
};

