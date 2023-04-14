#pragma once

#include "Menu.h"
#include "Transition.h"
#include "../Levels/Level.h"
#include "../GameObjects/Player.h"
#include "Setting.h"
#include "GameOver.h"
#include "LevelSelect.h"
#include <queue>

class PageManager {
public:
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
  LevelSelect *levelSelect;
  PageState state;
  queue<PageState> states;

  Player *player;
};


