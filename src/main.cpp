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
#include <memory>

using namespace std;


void runSDL();

void runSDL() {
  bool quit = false;

  // Load level

  gFont[NEXA_LIGHT] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 100);
  gFont[NEXA_BOLD] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 100);
  gFont[NEXA_LIGHT_10] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 30);
  gFont[NEXA_BOLD_10] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 50);
  gFont[NEXA_BOLD_20] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 200);
  if (gFont[0] == NULL || gFont[1] == NULL || gFont[2] == NULL || gFont[3] == NULL || gFont[4] == NULL)
    cout << "Failed to load font\n";

  Mix_Volume(-1, 127);
  Mix_VolumeMusic(127);
  // player.loadMedia();

  SDL_Event e;
  // Start the timer
  // gTimer.start();

  // Menu menu;
  // TransitionEffect trans;
  //
  PageManager *pm = new PageManager();
  GameOver *gameover = new GameOver();

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      // levelControl.handleKeyPress();
      // player.handleKeyPress();
      // menu.handleKeyPress(e);
      quit |= pm->handleKeyPressByState(e);
    }
    quit |= pm->handleWithoutEvent();

    // Clear screen
    // SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
    // SDL_RenderClear(gRenderer);

    // Application running
    // player.render();
    // levelControl.run(&player);
    // menu.render();

    // NOTE: Level control

    if(!quit) {
      pm->render();
    }

    // trans.render();
    // Update screen
    // SDL_RenderPresent(gRenderer);
    // SDL_Delay(12);
    // cout << "SDL_Error " << SDL_GetError() << endl;
  }

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
