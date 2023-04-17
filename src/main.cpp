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
#include "Menu/LevelSelect.h"

using namespace std;

void runSDL();

void runSDL() {
  // The frame per second timer
  LTimer fpsTimer;
  // The frame per second cap timer
  LTimer capTimer;
  // Start counting frames per second
  int countedFrames = 0;
  fpsTimer.start();

  bool quit = false;

  loadMedia();

  Mix_Volume(-1, 127);
  Mix_VolumeMusic(127);

  SDL_Event e;

  PageManager *pm = new PageManager(); 

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      quit |= pm->handleKeyPressByState(e);
      capTimer.start();
    }
    // TODO: Uncomment
    quit |= pm->handleWithoutEvent();
    
    // Calculate and correct fps
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
      avgFPS = 0;
    }

    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
    SDL_RenderClear(gRenderer);

    // Application running
    pm->render();

    // Update screen
    SDL_RenderPresent(gRenderer);
    ++countedFrames;

    //If frame finished early
    int frameTicks = capTimer.getTicks();
    if (frameTicks < SCREEN_TICKS_PER_FRAME) {
    // Wait remaining time
    SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
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
