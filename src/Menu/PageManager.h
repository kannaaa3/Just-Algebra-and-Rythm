#pragma once

#include "Menu.h"
#include "Transition.h"
#include "../Levels/Level.h"
#include "../GameObjects/Player.h"
#include "Setting.h"
#include "GameOver.h"
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
    GAME_OVER,
    TOTAL_STATES
  };
  PageManager();
  ~PageManager();

  void render();
  void changeStateInit(PageState nextState);
  void renderByState();
  bool handleKeyPressByState(SDL_Event e);
  bool handleWithoutEvent();

private:
  Menu *menu;
  TransitionEffect *transSqr;
  Level *levelControl;
  Setting *setting;
  GameOver *gameover;
  PageState state;
  queue<PageState> states;

  Player *player;
};


