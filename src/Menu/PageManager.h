#pragma once

#include "Menu.h"
#include "Transition.h"
#include "../Levels/Level.h"
#include "../GameObjects/Player.h"
#include <iostream>
#include <queue>
using namespace std;

class PageManager {
public:
  enum PageState {
    INGAME = 0,
    UNKNOWN,
    SETTING,
    EXIT,
    MENU,
    TRANSITION,
    TOTAL_STATES
  };
  PageManager();
  ~PageManager();

  void render();
  void changeStateInit(PageState nextState);
  void renderByState();
  bool handleKeyPressByState(SDL_Event e);

private:
  Menu *menu;
  TransitionEffect *transSqr;
  Level *levelControl;
  PageState state;
  queue<PageState> states;

  Player *player;
};


