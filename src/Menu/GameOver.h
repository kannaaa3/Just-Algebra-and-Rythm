#pragma once

#include "../constants.h"
#include "../util.h"

#include <iostream>
using namespace std;

class GameOver {
public:
  GameOver();
  ~GameOver();

  // Handle Key Press and return the next State
  int handleKeyPress(const SDL_Event e);
  void render();
  void refresh();

private:
  enum textID {
    YES,
    NO,
    TRY,
    GAMEOVER,
    TOTAL_TEXT
  };
  LTexture *text[TOTAL_TEXT];
  LTexture *selectedText[2];
  LTexture *selectEffect, *background;

  const vector<SDL_Point> pos= {{850, 589}, {1030, 589}, {960, 500}, {960, 327}};

  // By default, select NO
  int selectedSection;
  
};


