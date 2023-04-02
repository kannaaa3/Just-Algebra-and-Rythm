#include "PageManager.h"

PageManager::PageManager() {
  // Onlu init once and then recall all the time
  menu = new Menu();
  transSqr = new TransitionEffect();
  levelControl = new Level();
  player = new Player(500, 500);
  changeStateInit(MENU);
  states.push(MENU);
}

PageManager::~PageManager() {
}

void PageManager::render() {
  // Clear screen
  SDL_SetRenderDrawColor(gRenderer, BG.r, BG.g, BG.b, 0xFF);
  SDL_RenderClear(gRenderer);

  renderByState();
  // If in transition effect
  if (not transSqr->isRemovable()) {
    // if hiding current state page
    if (not transSqr->isFading()) {
      transSqr->render();
      // Update state
      if (transSqr->isFading()) {
        states.pop();
        changeStateInit(states.front());
      }
    } // If fading to next State
    else transSqr->fade();
  }

  // Update screen
  SDL_RenderPresent(gRenderer);
  SDL_Delay(12);
  // cout << "SDL_Error " << SDL_GetError() << endl;
}

void PageManager::changeStateInit(PageState nextState) {
  state = nextState;
  if (nextState == INGAME) {
    player->loadMedia();
    levelControl->loadMedia();
    levelControl->playMusic();
    // Start the timer
    gTimer.start();
  } 
  if (nextState == SETTING) {

  }
  if (nextState == MENU) {
    gMusic = Mix_LoadMUS(("assets/global/sound/Death_By_Glamour.wav"));
    if (gMusic == NULL) {
      printf("Failed to load BGM music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    menu->playMusic();
  }
}

void PageManager::renderByState() {
  // Application running
  switch (state) {
    case INGAME: {
      // Only render the player if in game
      player->render();
      levelControl->run(player);
      break;
    }
    case MENU: {
      menu->render();
      break;
    }
    case UNKNOWN: {
      break;
    }
    case SETTING: {
      break;
    }
  }
}

//Handle if there is State Transition
bool PageManager::handleKeyPressByState(SDL_Event e) {
  bool quit = false;
  switch (state) {
    case MENU: {
      int ans = menu->handleKeyPress(e);
      cout << "ans " << ans << endl;
    PageState nextState = static_cast<PageState>(ans);
    if (nextState != state) {
        // TODO: Make a transition here
        transSqr->refresh();
        states.push(nextState);
        cout << "PUSH QUEUE: " << nextState << endl;
        cout << "Queue: " ;
        queue<PageState> tmp = states;
        while (tmp.size()) {
          int i = tmp.front(); tmp.pop();
          cout << i <<" ";
        } cout << endl;
      }
    break;
    }
    case INGAME: {
      //TODO: If player ESC!!!
      player->handleKeyPress();
      levelControl->handleKeyPress();
      if (levelControl->trackCompleted()) {
        transSqr->refresh();
        states.push(MENU);
      }
    break;
    }
    case SETTING: {
    // TODO: Setting.cpp later
    break;
    }
    case EXIT: {
    states.pop();
    states.push(EXIT);
    quit = true;
    break;
    }
  }
  return quit;
}

