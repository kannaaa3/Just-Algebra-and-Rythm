#include "GameObjects/BouncingEntity.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/Player.h"
#include "Levels/Level.h"
#include "Menu/Menu.h"
#include "SDL_utils.h"
#include "constants.h"
#include "util.h"

#include <algorithm>
#include <iostream>

using namespace std;

Player player(500, 500);

void runSDL();

void runSDL() {
  bool quit = false;

  // Load level

  gFont[NEXA_LIGHT] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 100);
  gFont[NEXA_BOLD] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 100);
  gFont[NEXA_LIGHT_10] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 30);
  if (gFont[0] == NULL || gFont[1] == NULL || gFont[2] == NULL)
    cout << "Failed to load font\n";

  player.loadMedia();

  // NOTE: Level loading
  Level levelControl = Level();
  // levelControl.loadMedia("src/Levels/testLevel.txt");
  SDL_Event e;
  // Start the timer
  gTimer.start();

  Menu menu;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      levelControl.handleKeyPress();
      player.handleKeyPress();
      menu.handleKeyPress(e);
    }

    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
    SDL_RenderClear(gRenderer);

    // Application running
    // player.render();
    // levelControl.run(&player);
    menu.render();

    // NOTE: Level control

    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(12);
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
