#include "GameObjects/Enemy.h"
#include "GameObjects/MovingEntity.h"
#include "GameObjects/Player.h"
#include "GameObjects/SqrSnake.h"
#include "SDL_utils.h"
#include "constants.h"
#include "util.h"

#include <iostream>

using namespace std;

Player player(500, 500);
Enemy *m1;

void loadLevel();
void runSDL();

void runSDL() {
  bool quit = false;

  // Load level
  loadLevel();

  // r1.setAngleVel(2);
  // r1.setTimeNormal(10000);

  // SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 100, 100
  MovingEntity myM;
  myM.w = 80;
  myM.h = 80;
  myM.angle = 45;
  SqrSnake snk({SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 20}, 0, 180, 1);
  SqrSnake snk2({SCREEN_WIDTH, SCREEN_HEIGHT / 2 + 30}, 0, 180, 6, 30, 15);
  m1 = new Enemy(myM, {0, 2000, 600, 100000}, 255);

  m1->loadMedia();
  player.loadMedia();
  snk.loadMedia();
  snk.loadMediaSnk();
  snk2.loadMedia();
  snk2.loadMediaSnk();

  SDL_Event e;
  // Start the timer
  gTimer.start();
  m1->setStartTime(gTimer.getTicks());
  snk.setStartTime(gTimer.getTicks());

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (SDL_QUIT == e.type) {
        quit = true;
      }
      // quit |= homeHandleEvent(e);
      player.handleKeyPress();
    }

    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
    SDL_RenderClear(gRenderer);

    // Application running
    // homeScreen();

    player.render();

    m1->actByState();
    m1->render();
    snk.actByState();
    snk.render();
    snk2.actByState();
    snk2.render();

    if (m1->checkCollision(&player) || snk.checkCollision(&player)) {
      cout << "Trung \n";
    }

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

int main(int argc, char *argv[]) {
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    runSDL();
  }
  return 0;
}
