#include "PageManager.h"

PageManager::PageManager() {
  // Onlu init once and then recall all the time
  menu = new Menu();
  transSqr = new TransitionEffect();
  levelControl = new Level();
  setting = new Setting();
  gameover = new GameOver();
  levelSelect = new LevelSelect();

  player = new Player(500, 500);
  changeStateInit(MENU);
  states.push(MENU);
  player->loadMedia();
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
    // cout << "In transition effect" << endl;
    // if hiding current state page
    if (not transSqr->isFading()) {
      // cout << "Not fading" << endl;
      transSqr->render();
      // Update state
      if (transSqr->isFading()) {
        // cout << "Started fade" << endl;
        if (states.size() > 2) {
          cout << "HONG: size > 2" << endl;
          SDL_Delay(100000);
        }
        while(states.size() != 1) states.pop();
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
  cout << "Change state: " << nextState << endl;
  if (nextState == INGAME) {
    player->refresh();
    // TODO: Load media only once. Should I delete and re load or loaded all the time?
    levelControl->loadMedia();
    levelControl->playMusic();
    // Start the timer
    if (gTimer.isStarted()) gTimer.stop();
    gTimer.start();
  } 
  if (nextState == SETTING) {

  }
  if (nextState == MENU) {
    menu->refresh();
  // BouncingText work by Time, so need gTimer to start
    if (gTimer.isStarted()) gTimer.stop();
    gTimer.start();
  }
  if (nextState == GAME_OVER) {
    gameover->refresh();
  }
  if (nextState == LEVEL_SELECT) {
    if (gTimer.isStarted()) gTimer.stop();
    gTimer.start();
    player->refresh();
    levelSelect->refresh(player);
  }
}

void PageManager::renderByState() {
  // cout << "Application rendering " << state << endl;
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
    case GAME_OVER: {
      gameover->render();
      break;
    }
    case SETTING: {
      setting->render();
      break;
    }
    case LEVEL_SELECT: {
      levelSelect->render(player);
      player->render();
      break;
    }
  }
}

//Handle if there is State Transition
bool PageManager::handleKeyPressByState(SDL_Event e) {
  bool quit = false;
  switch (state) {
    case MENU: {
    PageState nextState = static_cast<PageState>(menu->handleKeyPress(e));
    if (nextState != state) {
        transSqr->refresh();
        states.push(nextState);
      }
    break;
    }
    case INGAME: {
      //TODO: If player ESC!!!
      if (levelControl->trackCompleted()) {
        if (states.size() < 2) {
          Mix_HaltMusic();
          transSqr->refresh();
          states.push(LEVEL_SELECT);
          cout << "Push LevelSelect after track complete:" << endl;
        }
      } else {
        if (player->isDead()) {
          // TODO: IT'S NOT OVER SCREEN
          if (states.size() < 2) {
            Mix_HaltMusic();
            transSqr->refresh();
            states.push(GAME_OVER);
          }
        } else if(states.size() < 2) {
          player->handleKeyPress();
          PageState nextState = static_cast<PageState> (levelControl->handleKeyPress(e));
          if (nextState != state) {
            gTimer.stop();
            Mix_HaltMusic();
            transSqr->refresh();
            states.push(nextState);
          }
        }
      }
    break;
    }
    case SETTING: {
    // TODO: Setting.cpp later
    bool back = setting->handleKeyPress(e);
    if (back) {
        transSqr->refresh();
        states.push(MENU);
      }
    break;
    }
    case GAME_OVER: {
      PageState nextState = static_cast<PageState>(gameover->handleKeyPress(e));
      if (nextState != state) {
        Mix_HaltMusic();
        transSqr->refresh();
        states.push(nextState);
      }
      break;
    }
    case UNKNOWN: {
    // TODO: ESC to back
      break;
    }
    case EXIT: {
    states.pop();
    states.push(EXIT);
    changeStateInit(EXIT);
    quit = true;
      cout << "Set quit true" << endl;
    break;
    }
    case LEVEL_SELECT: {
    int selected_song = levelSelect->handleKeyPress(e, player);
    if (selected_song != -1 && selected_song != MENU) {
        // TODO: Change state, load song
        Mix_HaltMusic();
        transSqr->refresh();
        levelControl->setCurrentLevel(selected_song);
        states.push(INGAME);
      }
    else  if (selected_song == MENU) {
        Mix_HaltMusic();
        transSqr->refresh();
        states.push(MENU);
      }
    break;
    }
  }
  return quit;
}

bool PageManager::handleWithoutEvent(){
  switch (state) {
      case MENU: {
          // TODO: Make a transition here

      break;
      }
    case INGAME: {
      //TODO: If player ESC!!!
      if (levelControl->trackCompleted()) {
        if (states.size() < 2) {
          transSqr->refresh();
          cout << "Push Level Select after track complete:" << endl;
          states.push(LEVEL_SELECT);
        }
      } else {
        if (player->isDead()) {
            if (states.size() < 2) {
              Mix_HaltMusic();
              transSqr->refresh();
              states.push(GAME_OVER);
          }
        } 
      }
    break;
    }
    case EXIT: {
      return true;
    break;
    }
  }
  return false;
}

