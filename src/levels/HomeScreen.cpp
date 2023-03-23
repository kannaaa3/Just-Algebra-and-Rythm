#include "HomeScreen.h"
#include <iostream>

using namespace std;

enum soundID {
  BGM_HOME_SCREEN = 0,
  NAVIGATE_MENU,
  SELECT_MENU,
  TOTAL_SOUND
};

enum menuType {
  MENU_NEW_GAME = 0,
  MENU_PLAYLIST,
  MENU_QUIT
};

vector<string> fileName = {
  "new_game_unselected.png", "new_game_selected.png", 
  "playlist_unselected.png", "playlist_selected.png",
  "quit_unselected.png", "quit_selected.png",
  "fred_eddy_tree.png"
};

// Position of texture
vector<pair<int,int>> pos = {
  {930, 360}, {849, 360},
  {930, 448},  {849, 448},
  {930, 536}, {849, 536},
  {50, 0},
};

vector<string> musicName = {
  "Tien_Quan_Ca_Fred_Eddy.wav"
};


bool loadHomeScreen() {
  bool success = true;

  string prefix = "../assets/HomeScreen/";
  allTextures.resize(fileName.size());
  for (int i = 0; i < fileName.size(); i++) {
    if (!allTextures[i].loadFromFile(gRenderer, prefix + "img/" + fileName[i])) {
      printf( "First time: Failed to load %s texture image!\n", (prefix + "img/" + fileName[i]).c_str());
      success = false;
    }
    allTextures[i].setPosition(pos[i].first, pos[i].second);
  }

  gMusic = Mix_LoadMUS((prefix + "sound/" + musicName[0]).c_str());
  if (NULL == gMusic ) {
    printf( "Failed to load %s music! SDL_mixer Error: %s\n", musicName[0].c_str(), Mix_GetError() );
    success = false;
  }
  
  return success;
}

// Return true if quit, else false
bool homeHandleEvent(SDL_Event &e) {
  bool quit = false;
  if (SDL_KEYDOWN == e.type) {
    switch (e.key.keysym.sym) {
      case SDLK_UP:
        if (gMenuID > 0) gMenuID--;
        break;
      case SDLK_DOWN:
        if (gMenuID < 2) gMenuID++;
        break;
      case SDLK_RETURN:
        // TODO: Add transition function here
        if (gMenuID == MENU_NEW_GAME) {
          gLevel = 2;
        } 
        if (gMenuID == MENU_PLAYLIST) {

        }
        if (gMenuID == MENU_QUIT) quit = true;
    }
  }
  return quit;
}

void homeScreen(){
  // Menu texture renderring
  for (int i = 0; i < 3; i++) {
    int id = (i << 1) + 1 * (gMenuID == i);
    allTextures[id].render(gRenderer, allTextures[id].getX(), allTextures[id].getY());
  }

  // Texture rendering
  for (int id = 6; id < allTextures.size(); id++) {
    allTextures[id].render(gRenderer, allTextures[id].getX(), allTextures[id].getY());
  }

  // Load BGM
  if(Mix_PlayingMusic() == 0)
    Mix_PlayMusic(gMusic, -1);
}
