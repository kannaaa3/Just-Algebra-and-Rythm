#include "GameObjects/Enemy.h"
#include "GameObjects/MovingEntity.h"
#include "GameObjects/Player.h"
#include "GameObjects/SqrSnake.h"
#include "SDL_utils.h"
#include "constants.h"
#include "util.h"
#include "GameObjects/BouncingEntity.h"

#include <iostream>

using namespace std;

Player player(500, 500);
Enemy *m1;

void runSDL();

void runSDL() {
  bool quit = false;

  // Load level

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

  // Test bounce text
  gFont = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 100);
  if (gFont == NULL) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
  }
  TTF_Font* nexaBold = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 100);
  if (nexaBold == NULL) {
    printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
  }

  BouncingText myText("I SEE, YOU'RE NEW", gFont, SCREEN_WIDTH/2, 200, 1350, 5170),
  my2Text("DON'T WORRY", gFont, SCREEN_WIDTH/2, 200, 5180, 8560),
  my3Text("LET'S GET STARTED",nexaBold, SCREEN_WIDTH/2, 200, 8560, 14640 );
  gMusic = Mix_LoadMUS("assets/cYsmix - Peer Gynt.wav");
  if(gMusic == NULL) {
    printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
  }
  if(Mix_PlayingMusic() == 0) Mix_PlayMusic(gMusic, -1);

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

    myText.render();
    my2Text.render();
    my3Text.render();

    if (m1->checkCollision(&player) || snk.checkCollision(&player)) {
      cout << "Trung \n";
    }

    // Update screen
    SDL_RenderPresent(gRenderer);
    SDL_Delay(12);
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
