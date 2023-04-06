#include "GameObjects/BouncingEntity.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Player.h"
#include "Levels/Level.h"
#include "Menu/Menu.h"
#include "Menu/Transition.h"
#include "SDL_utils.h"
#include "constants.h"
#include "util.h"
#include "Menu/PageManager.h"
#include "Menu/Setting.h"
#include "Menu/GameOver.h"

#include <algorithm>
#include <iostream>

using namespace std;


void runSDL();

void runSDL() {
  bool quit = false;

  const string fontType[] = {"assets/global/fonts/nexa-light.otf", "assets/global/fonts/nexa-bold.otf"};
  const int type[] = {0, 1, 0, 1, 1};
  const int fontSz[] = {100, 100, 30, 50, 200};
  for(int i = 0; i < TOTAL_FONT; i++) {
    gFont[i] = TTF_OpenFont(fontType[type[i]].c_str(), fontSz[i]);
    if (NULL == gFont[i]) 
      cout << "Failed to load font " << fontType[type[i]] << endl;;
  }

  Mix_Volume(-1, 127);
  Mix_VolumeMusic(127);

  SDL_Event e;
  // Start the timer
  gTimer.start();

  PageManager *pm = new PageManager(); 

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      // levelControl.handleKeyPress();
      // player.handleKeyPress();
      // TODO: Uncomment
      quit |= pm->handleKeyPressByState(e);
    }
    // TODO: Uncomment
    quit |= pm->handleWithoutEvent();

    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
    SDL_RenderClear(gRenderer);

    // Application running
    // player.render();
    // levelControl.run(&player);


    if(!quit) {
      //TODO: Uncomment
      pm->render();
    }

    // Update screen
    SDL_RenderPresent(gRenderer);
    // SDL_Delay(12);
  }
  // delete track;
  quitSDL();
}

int main(int argc, char *argv[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    runSDL();
  }
  return 0;
}
