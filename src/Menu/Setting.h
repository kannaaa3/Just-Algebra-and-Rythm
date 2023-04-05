// Mix_Volume(-1, 127);
// Mix_VolumeMusic(127);
#pragma once

#include "../constants.h"
#include "../util.h"

#include <iostream>
using namespace std;

class Setting {
public:
  Setting();
  ~Setting();

  bool handleKeyPress(const SDL_Event e);
  void render();

  void refresh();

private:
  const int OFFSET = 127/50;
  enum TextID {
    TITLE = 0,
    MUSIC,
    SOUND_FX,
    TOTAL_TEXT
  };
  enum TextCOLOR {
    WHITE_ID = 0,
    BLUE_ID,
    TOTAL_COL
  };
  LTexture *textTexture[TOTAL_TEXT][2];
  const vector<SDL_Point> pos = {{187, 42}, {241, 260}, {241, 360}};

  int selected_section;
  int vol[TOTAL_TEXT];
  LTexture *spinSqr;
  float angle = 0;

  Mix_Chunk *sfxNavigate, *sfxAdjust;

};
