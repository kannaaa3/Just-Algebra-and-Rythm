#include "Menu.h"

Menu::Menu() {
  // If not, core dump
  spinSqr = new LTexture();
  logo = new LTexture();
  logoSplash = new LTexture();
  for(int i = 0; i < TOTAL_MENU_ID; i++)
    menuSFX[i] = NULL;

  const vector<string> sectionName = {"New Game", "???", "Setting", "Exit"};
  const vector<SDL_Point> pos = {{200, 100}, {707,100}, {1213, 100}, {1720, 100}};
  for (int i = 0; i < TOTAL_SECTION; i++) {
    section[i].textTexture.loadFromRenderedText(gRenderer, gFont[NEXA_LIGHT_10], sectionName[i], {0xFF, 0xFF, 0xFF});
    section[i].textTexture.setPosition(pos[i].x, pos[i].y+ 80);
    section[i].x = pos[i].x;
    section[i].y = pos[i].y;
    section[i].bounceText = new BouncingText(sectionName[i], NEXA_LIGHT_10, pos[i].x, pos[i].y+ 80, 0, 6000000);
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
  string prefix = "assets/global/sound/";
  vector<string> sfxName = {"menu_select.wav", "setting_sfx.wav", "transition.wav"};
  for(int i = 0; i < TOTAL_MENU_ID; i++) {
    menuSFX[i] = Mix_LoadWAV((prefix + sfxName[i]).c_str());
    if(menuSFX[i]== NULL) {
      printf( "Failed to load %s! SDL_mixer Error: %s\n", sfxName[i].c_str(), Mix_GetError());
    }
  }
  // Load the BGM
  gMusic = Mix_LoadMUS(("assets/global/sound/Death_By_Glamour.wav"));
  if (gMusic == NULL) {
    printf("Failed to load BGM music! SDL_mixer Error: %s\n", Mix_GetError());
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

int Menu::handleKeyPress(SDL_Event e) {
  cout << "1" << endl;
  if (e.type == SDL_KEYDOWN) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
      if (currentSection > NEW_GAME)
        currentSection--;
      Mix_PlayChannel(-1, menuSFX[SELECT], 0);

      break;
      case SDLK_RIGHT:
      if (currentSection < EXIT)
        currentSection++;
      Mix_PlayChannel(-1, menuSFX[SELECT], 0);
      break;
      case SDLK_RETURN:
      // TODO: DO something
      if (currentSection == NEW_GAME) {
          if(Mix_PlayingMusic() != 0) Mix_HaltMusic();
          Mix_PlayChannel(-1, menuSFX[ENTERGAME], 0);
        } else Mix_PlayChannel(-1, menuSFX[ENTER], 0);
      cout << "What " << currentSection << endl;
      return currentSection;
      break;
    }
  }
  cout << "Total " << TOTAL_SECTION << endl;
  return TOTAL_SECTION;
}

void Menu::render() {
  angle += ANGLE_VEL;

  // Render the logo
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

void Menu::playMusic() {
  // Play the BGM
  if(!Mix_PlayingMusic()) {
    Mix_PlayMusic(gMusic, -1);
  }
}
