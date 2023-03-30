#include "GameObjects/BouncingEntity.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/MovingEntity.h"
#include "GameObjects/Player.h"
#include "GameObjects/SqrSnake.h"
#include "Levels/Level.h"
#include "SDL_utils.h"
#include "constants.h"
#include "util.h"

#include <algorithm>
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
  // MovingEntity myM;
  // myM.w = 80;
  // myM.h = 80;
  // myM.angle = 45;
  // SqrSnake snk(0, {SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 20}, 180, 1);
  // SqrSnake snk2(0, {SCREEN_WIDTH, SCREEN_HEIGHT / 2 + 30}, 180, 6, 30, 15);
  // m1 = new Enemy(myM, {0, 2000, 600, 100000}, 255);

  gFont[NEXA_LIGHT] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 100);
  gFont[NEXA_BOLD] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 100);
  if (gFont[0] == NULL || gFont[1] == NULL) cout << "Failed to load font\n";
  // Test bounce text
  // BouncingText myText("I SEE, YOU'RE NEW", gFont[NEXA_LIGHT], SCREEN_WIDTH / 2,
  //                     200, 1350, 5170),
  //     my2Text("DON'T WORRY", gFont[NEXA_LIGHT], SCREEN_WIDTH / 2, 200, 5180,
  //             8560),
  //     my3Text("LET'S GET STARTED", gFont[NEXA_BOLD], SCREEN_WIDTH / 2, 200,
  //             8560, 14640);
  gMusic = Mix_LoadMUS("assets/cYsmix - Peer Gynt.wav");
  if (gMusic == NULL) {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }
  if (Mix_PlayingMusic() == 0)
    Mix_PlayMusic(gMusic, -1);

  player.loadMedia();

  Level levelControl = Level();
  levelControl.loadMedia("src/Levels/testLevel.txt");

  cout << "All reference of textTexture:" << endl;
  for(int i = 0; i < levelControl.txtRender.size(); i++) {
    cout << "# " << i <<": " << levelControl.txtRender[i].textTexture << " " << gRenderer << endl;
  }


  cout << SDL_GetError() << endl;
  for (auto &r : levelControl.snkRender) r.loadMedia();
  for (auto &r : levelControl.enemyRender) r.loadMedia();
  // for (auto &r : levelControl.snkRender) cout << r.getStartTime() << endl;
  // for (auto &r : levelControl.txtRender) r.loadMedia();
  // for (auto &r : levelControl.txtRender) cout << r.getStartTime() << endl;
  
  // for(int i = 0; i < levelControl.txtRender.size(); i++) {
  //   if (!levelControl.txtRender[i].textTexture.loadFromRenderedText(gRenderer, gFont[levelControl.fontID[i]], levelControl.txtVRender[i], ALLY_BLUE)) 
  //   cout << "Hong r!\n";
  // }
  //
  // TODO: Sort all the texture base on start time
  // sort(levelControl.txtRender.begin(), levelControl.txtRender.end(), 
  //      [] (BouncingText a, BouncingText b) -> bool {
  //        return a.startTime < b.startTime;
  //      });
  

  SDL_Event e;
  // Start the timer
  gTimer.start();

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
    player.render();

    while (levelControl.snkRender.size() &&
           levelControl.snkRender.front().isRemovable())
      levelControl.snkRender.pop_front();
    for (int i = 0; i < levelControl.snkRender.size(); i++) {
      if (levelControl.snkRender[i].getStartTime() > gTimer.getTicks())
        break;
      levelControl.snkRender[i].render();
    }

    while (levelControl.txtRender.size() &&
           levelControl.txtRender.front().isRemovable())
      levelControl.txtRender.pop_front();

    for (int i = 0; i < levelControl.txtRender.size(); i++) {
      if (levelControl.txtRender[i].getStartTime() > gTimer.getTicks())
        break;

      levelControl.txtRender[i].render();

      
    }

    while (levelControl.enemyRender.size() &&
           levelControl.enemyRender.front().isRemovable())
      levelControl.enemyRender.pop_front();
    for (int i = 0; i < levelControl.enemyRender.size(); i++) {
      if (levelControl.enemyRender[i].getStartTime() > gTimer.getTicks())
        break;
      levelControl.enemyRender[i].render();
    }
    // if (m1->checkCollision(&player) || snk.checkCollision(&player)) {
    //   cout << "Trung \n";
    // }

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
