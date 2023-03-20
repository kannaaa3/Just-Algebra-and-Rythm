#include "util.h"
#include "constants.h"
#include "SDL_utils.h"
#include "GameObjects/Player.h"
#include "levels/HomeScreen.h"
#include "GameObjects/SpinningRect.h"

using namespace std;

Player player(500, 500);

void loadLevel();
void runSDL();

void runSDL() {
  bool quit = false;

  // Load level
  loadLevel();

  SDL_Point winpos = {200, 200};

  SDL_Event e;
  gTimer.start();

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      // quit |= homeHandleEvent(e);
      player.handleKeyPress();
    }

    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b , 0xFF);
    SDL_RenderClear(gRenderer);

    // Application running
    // homeScreen();

    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(12);
  }

  quitSDL();
}

void loadLevel() {
    switch (gLevel) {
      case 0:
        loadHomeScreen();
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
    }
}

int main (int argc, char *argv[])
{
  if (!init()) {
    printf("Failed to initialize!\n");
  }
  else {
    runSDL();
  }
  return 0;
}
