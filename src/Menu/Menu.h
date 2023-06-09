#pragma once

#include "../util.h"
#include "../GameObjects/BouncingEntity.h"
#include <vector>
using namespace std;

class Menu {
public:
  enum SECTION {
    NEW_GAME = 0,
    UNKNOWN,
    SETTING,
    EXIT,
    TOTAL_SECTION
  };
  Menu();
  ~Menu();

  int handleKeyPress(SDL_Event e);
  void render();

  void refresh();

private:
  const int SIZE = 40, LOGO_SCALE = 20, TIME_SCALE_DELAY = 200;
  struct Section {
    int x, y;
    LTexture textTexture;
    BouncingText* bounceText;
  } section[TOTAL_SECTION];

  enum MenuID {
    SELECT = 0,
    ENTER,
    ENTERGAME,
    TOTAL_MENU_ID
  };

  LTexture *spinSqr, *logo, *logoSplash;
  float angle = 0, logoAngle = 0;
  const float ANGLE_VEL = 3;
  int scale, dir, logoW, logoH;
  LTimer scaleDelay;

  Mix_Chunk *menuSFX[TOTAL_MENU_ID];
  Mix_Music *bgm;

  int currentSection;
};

