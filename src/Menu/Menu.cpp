#include "Menu.h"

Menu::Menu() {
  // If not, core dump
  spinSqr = new LTexture();
  logo = new LTexture();
  logoSplash = new LTexture();
  menuSelect = NULL;
  menuEnter = NULL;

  const vector<string> sectionName = {"New Game", "???", "Setting", "Exit"};
  const vector<pair<int, int>> sectionPos = {{200, 100}, {707,100}, {1213, 100}, {1720, 100}};
  for (int i = 0; i < TOTAL_SECTION; i++) {
    section[i].textTexture.loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], sectionName[i], {0xFF, 0xFF, 0xFF});
    section[i].textTexture.setPosition(sectionPos[i].first, sectionPos[i].second + 80);
    section[i].x = sectionPos[i].first;
    section[i].y = sectionPos[i].second;
    section[i].bounceText = new BouncingText(sectionName[i], NEXA_LIGHT_10, sectionPos[i].first, sectionPos[i].second + 80, 0, 6000000);
  }

  if (!spinSqr->loadFromFile(gRenderer, "assets/GameObjects/img/PlayerIdle.png")) {
    printf("Failed to load spinSqr! SDL_Error: %s", SDL_GetError());
  } else {
    spinSqr->setDimension(SIZE+10, SIZE+10);
  }
  if (!logo->loadFromFile(gRenderer, "assets/global/img/logo_JustAlgRythm.png")) {
    printf("Failed to load Logo! SDL_Error: %s", SDL_GetError());
  } else {
    // spinSqr->setDimension(SIZE+10, SIZE+10);
    logoW = logo->getWidth();
    logoH = logo->getHeight();
  }
  if (!logoSplash->loadFromFile(gRenderer, "assets/global/img/logoSplash.png")) {
    printf("Failed to load LogoSplash! SDL_Error: %s", SDL_GetError());
  }

  menuSelect = Mix_LoadWAV("assets/global/sound/menu_select.wav");
  if(menuSelect== NULL) {
    printf( "Failed to load menu select sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
  }
  menuEnter = Mix_LoadWAV("assets/global/sound/setting_sfx.wav");
  if(menuEnter== NULL) {
    printf( "Failed to load menu enter sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
  }


  gMusic = Mix_LoadMUS(("assets/global/sound/Death_By_Glamour.wav"));
  if (gMusic == NULL) {
    printf("Failed to load BGM music! SDL_mixer Error: %s\n", Mix_GetError());
  }

  if(!Mix_PlayingMusic()) {
    Mix_PlayMusic(gMusic, -1);
  }

  currentSection = NEW_GAME;
  scale = 0;

}

Menu::~Menu() { 
  spinSqr->free();
  for(int i = 0; i != TOTAL_SECTION; i++) {
    section[i].bounceText->textTexture->free();
    section[i].textTexture.free();
  }
  
}

void Menu::handleKeyPress(const SDL_Event e) {
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
      if (currentSection > NEW_GAME)
        currentSection--;
      Mix_PlayChannel(-1, menuSelect, 0);

      break;
      case SDLK_RIGHT:
      if (currentSection < EXIT)
        currentSection++;
      Mix_PlayChannel(-1, menuSelect, 0);
      break;
      case SDLK_RETURN:
      // TODO: DO something
      Mix_PlayChannel(-1, menuEnter, 0);
      break;
    }
  }
}

void Menu::render() {
  angle += ANGLE_VEL;
  logoAngle += 1;
  if (scale == 0) {
    if (scaleDelay.isStarted()) {
      if (scaleDelay.getTicks() >= TIME_SCALE_DELAY) {
        dir = 1;
        scaleDelay.stop();
      }
    } else {
      dir = 0;
      scaleDelay.start();
    }
  } else if(scale > LOGO_SCALE ) {
    dir = -1;
  }
  scale += dir;
  
  // Clear scr
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
  
  logoSplash->renderCenter(gRenderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2,SCREEN_DIAGONAL, SCREEN_DIAGONAL, NULL, logoAngle);
  logo->setDimension(logoW + logoW * scale/150, logoH + logoH * scale / 150);
  logo->renderCenter(gRenderer, 960, 540, logo->getWidth(), logo->getHeight());

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 10);
  for(int i = 200; i < 1720; i+=6) {
    SDL_RenderDrawPoint( gRenderer, i  , 100);    
    SDL_RenderDrawPoint( gRenderer, i+1, 100);    
  }
  
  SDL_SetRenderDrawColor(gRenderer, 9, 124, 132, 0xff);
  for(int i = NEW_GAME; i != TOTAL_SECTION; i++) {
    if (currentSection == i) {
      // TODO: Render rectangle and spinning rectangle
      spinSqr->renderCenter(gRenderer, section[i].x, section[i].y, SIZE, SIZE, NULL, angle);
      section[i].bounceText->render();
    } else {
      SDL_Rect r = {section[i].x - SIZE / 2, section[i].y - SIZE / 2, SIZE, SIZE};
      SDL_RenderFillRect(gRenderer, &r);
      // Render text below
      LTexture* textTexture = &section[i].textTexture;
      section[i].textTexture.renderCenter(gRenderer, textTexture->getX(), textTexture->getY(), textTexture->getWidth(), textTexture->getHeight());
    }
  }
  
}
