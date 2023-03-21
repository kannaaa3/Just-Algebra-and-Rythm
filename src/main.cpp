#include "util.h"
#include "constants.h"
#include "SDL_utils.h"
#include "GameObjects/Player.h"
// #include "levels/HomeScreen.h"
#include "GameObjects/SpinningRect.h"
#include "GameObjects/MovingEntity.h"

using namespace std;

Player player(500, 500);
SpinningRect r1, r2;
MovingEntity* m1;

void loadLevel();
void runSDL();

void runSDL() {
  bool quit = false;

  // Load level
  loadLevel();
  
  r1.setAngleVel(2);
  r1.setTimeNormal(10000);

  m1 = new MovingEntity(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100, 100);
  // m1->setAngleVel(2);
  // m1->setNormalTime(5000);
  // m1->setDimension(SCREEN_WIDTH, 30);

  m1->loadMedia();
  r1.loadMedia();
  player.loadMedia();


  SDL_Point winpos = {200, 200};

  SDL_Event e;
  // Start the timer
  gTimer.start();
  m1->setStartTime(gTimer.getTicks());

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

    m1->actByState();
    // r1.actByState();
    // r1.render(); // NOTE: Not render r1
    m1->render();
    player.render();
    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(12);
  }

  quitSDL();
}

void loadLevel() {
    switch (gLevel) {
      case 0:
        // loadHomeScreen();
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
