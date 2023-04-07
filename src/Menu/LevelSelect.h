#include "../util.h"
#include "../GameObjects/Player.h"

#include <iostream>
using namespace std;

class LevelSelect {
public:
  LevelSelect();
  ~LevelSelect();

  void handleKeyPress(const SDL_Event e);
  void render(Player *p);
  void refresh();

private:
  const int SECTION_HEIGHT = SCREEN_HEIGHT/ TOTAL_SONGS;
  Player* p;

  struct SongSection {
    LTexture *name, *author;
    Mix_Music *music;
  } songSection[TOTAL_SONGS];

  // based on y coordinate
  int selected_song;
  
};

