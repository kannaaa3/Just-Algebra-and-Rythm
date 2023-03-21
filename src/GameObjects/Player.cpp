#include "Player.h"
#include <iostream> // TODO: Delete
using namespace std;

Player::Player(int x, int y) {
  pPosX = x;
  pPosY = y;
  pAngle = 0;
  setState(IDLE);
  currentSpriteID = 0;
  lastDashTime = -DASH_COOLDOWN;
  velocity = 0;

  // Splash
}

Player::~Player() {
}

void Player::loadMedia() {
  SDL_Rect tmpRect;
  tmpRect.w = P_SIZE;
  tmpRect.h = P_SIZE;
  tmpRect.y = 0;

  string prefix = "../assets/GameObjects/img/";
  if (!pSpriteTextures[IDLE].loadFromFile(gRenderer, prefix + "PlayerIdle.png")) 
    printf("Failed to load PlayerIdle texture!\n");

  if (!pSpriteTextures[IDLE_TO_MOVE].loadFromFile(gRenderer, prefix + "PlayerIdleToMoveSprites.png"))
    printf("Failed to load PlayerIdleToMove texture!\n");

  if (!pSpriteTextures[MOVE].loadFromFile(gRenderer, prefix + "PlayerMove.png"))
    printf("Failed to load PlayerMove texture!\n");

  if (!pSpriteTextures[MOVE_TO_IDLE].loadFromFile(gRenderer, prefix + "PlayerMoveToIdleSprites.png"))
    printf("Failed to load PlayerMoveToIdle texture!\n");
  
  if (!pSpriteTextures[DASH].loadFromFile(gRenderer, prefix + "PlayerDashSprites.png")) 
    printf("Failed to load PlayerDash texture!\n");


  for (int i = IDLE; i != TOTAL_STATES; i++) {
    for (int j = 0; j * pSpriteTextures[i].getHeight() < pSpriteTextures[i].getWidth(); j++) {
      tmpRect.x = P_SIZE * j;
      playerSprites[i].push_back(tmpRect);
    }
  }
}


void Player::handleKeyPress() {
  // if dashing, not handle any key press
  if (pState == DASH) return;

  Uint8 keyPress[] = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_DOWN};
  float angleKeyPress[] = {0, 90, 180, 270};

  const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
  for (int i = 0; i <= 4; i++) {
    // if a key was press, or the player MOVE
    if (i != 4 && currentKeyStates[keyPress[i]]) {
      // set angle
      pAngle = angleKeyPress[i];
      if (currentKeyStates[keyPress[(i + 1)%4]]) pAngle += 45.0;
      else if (currentKeyStates[keyPress[(i+3)%4]]) pAngle -= 45.0;
      if (pAngle < 0) pAngle += 360.0;

      // NOTE: Add splash
      SDL_Rect tmpRect;
      tmpRect.x = randomNumber(pPosX - P_SIZE/2 + 15, pPosX + P_SIZE/2 - 15);
      tmpRect.y = randomNumber(pPosY - P_SIZE/2 + 15, pPosY + P_SIZE/2 - 15);
      tmpRect.w = 20; tmpRect.h = 20;
      splashStates.push_back({tmpRect, static_cast<float>(fmod(pAngle+180, 360)), 180});

      tmpRect.x = randomNumber(pPosX - P_SIZE/2 + 15, pPosX + P_SIZE/2 - 15);
      tmpRect.y = randomNumber(pPosY - P_SIZE/2 + 15, pPosY + P_SIZE/2 - 15);
      tmpRect.w = 20; tmpRect.h = 20;
      splashStates.push_back({tmpRect, static_cast<float>(fmod(pAngle+180, 360)), 180});
      /////////////////////////////////////////////////////////////////////////////

      switch (pState) {
        case IDLE:
        // So that it could add to 0
          currentSpriteID = -1;
          setState(IDLE_TO_MOVE);
          break;
        case MOVE_TO_IDLE:
        // So that it could add to 0
          currentSpriteID = -1;
          setState(IDLE_TO_MOVE);
          break;
      }

      // TODO: Set 2 random squares splash
      //
      // Stop iterating
      break;
    }
    // none of the key was pressed
    if (i == 4) {
      switch (pState) {
        case MOVE:
          currentSpriteID = -1;
          setState(MOVE_TO_IDLE);
          break;
        case IDLE_TO_MOVE:
          currentSpriteID = -1;
          setState(MOVE_TO_IDLE);
          break;
      }
    }
  }

  // If player dash
  if (currentKeyStates[SDL_SCANCODE_SPACE] && DASH_COOLDOWN < gTimer.getTicks() - lastDashTime) {
    // cout << "Time minus = " << gTimer.getTicks() - lastDashTime << endl;
    currentSpriteID = -1;
    setState(DASH);
  }

}

void Player::actByState() {
  switch (pState) {
    case IDLE:
      idle();
      break;
    case IDLE_TO_MOVE:
      idleToMove();
      break;
    case MOVE:
      move();
      break;
    case MOVE_TO_IDLE:
      moveToIdle();
      break;
    case DASH:
      dash();
      break;
  }
}

void Player::idle() {
  if (currentSpriteID >= playerSprites[IDLE].size()) {
    currentSpriteID = 0;
  }
}

void Player::idleToMove() {
  if (currentSpriteID + 1  >= numFrame[IDLE_TO_MOVE] * playerSprites[IDLE_TO_MOVE].size())  {
    // TODO:  change to move
    setState(MOVE);
    // cout << "Change to MOVE\n";
    currentSpriteID = 0;
  } else {
    currentSpriteID++;
  }

    int shiftX = P_VEL * cos(pAngle / 180 * M_PI);
    int shiftY = -P_VEL * sin(pAngle / 180 * M_PI);

    // If not go out of screen
    if (not (pPosX + shiftX < P_SIZE/2 + 1  || pPosX + shiftX + P_SIZE/2 > SCREEN_WIDTH || pPosY + shiftY < P_SIZE/2 + 1|| pPosY + shiftY + P_SIZE/2 > SCREEN_HEIGHT)) {
      pPosX += shiftX;
      pPosY += shiftY;
    }

}

void Player::move() {
  if (currentSpriteID >= playerSprites[MOVE].size()) {
    currentSpriteID = 0;
  }

  // Move the player
  int shiftX = P_VEL * cos(pAngle / 180 * M_PI);
  int shiftY = -P_VEL * sin(pAngle / 180 * M_PI);

  // If not go out of screen
  if (not (pPosX + shiftX < P_SIZE/2 + 1  || pPosX + shiftX + P_SIZE/2 > SCREEN_WIDTH || pPosY + shiftY < P_SIZE/2 + 1|| pPosY + shiftY + P_SIZE/2 > SCREEN_HEIGHT)) {
    pPosX += shiftX;
    pPosY += shiftY;
  }
  
}

void Player::moveToIdle() {
  if (currentSpriteID + 1 >= numFrame[MOVE_TO_IDLE] * playerSprites[MOVE_TO_IDLE].size()) {
    setState(IDLE);
    currentSpriteID = 0;
  } else {
    currentSpriteID++;
  }
}

void Player::dash() {
  if (currentSpriteID + 1 >= numFrame[DASH] * playerSprites[DASH].size()) {
    // TODO: Stop dashing and set dash cool down
    lastDashTime = gTimer.getTicks();
    setState(MOVE);
    currentSpriteID = 0;
    //NOTE: If not, it will still move even though no key press
    handleKeyPress();
  } else {
    // TODO: Continue dashing
    currentSpriteID++;

    // Move the player
    int shiftX = DASH_VEL * cos(pAngle / 180 * M_PI);
    int shiftY = -DASH_VEL * sin(pAngle / 180 * M_PI);

    // If not go out of screen
    if (not (pPosX + shiftX < P_SIZE/2 + 1  || pPosX + shiftX + P_SIZE/2 > SCREEN_WIDTH || pPosY + shiftY < P_SIZE/2 + 1|| pPosY + shiftY + P_SIZE/2 > SCREEN_HEIGHT)) {
      pPosX += shiftX;
      pPosY += shiftY;
    }
  }
}

void Player::splash() {
  while(splashStates.size() && splashStates.front().rect.w < 1) 
    splashStates.pop_front();

  LTexture tmpTexture;

  // Load repeatedly
  string prefix = "../assets/GameObjects/img/";
  if (!tmpTexture.loadFromFile(gRenderer, prefix + "PlayerIdle.png")) 
    printf("Failed to load PlayerIdle texture!\n");

  for (int i = 0; i < splashStates.size(); i++) {
    int shiftX =  2 * cos(splashStates[i].angle / 180 * M_PI);
    int shiftY = -2 * sin(splashStates[i].angle / 180 * M_PI);
    SDL_Rect spr;
    spr.w = splashStates[i].rect.w;
    spr.h = splashStates[i].rect.h;

    tmpTexture.setAlpha(splashStates[i].alpha);
    tmpTexture.renderCenter(gRenderer, splashStates[i].rect.x, splashStates[i].rect.y, spr.w, spr.h, &spr, splashStates[i].angle);

    splashStates[i].rect.w -= 1;
    splashStates[i].rect.h -= 1;
    splashStates[i].rect.x += shiftX;
    splashStates[i].rect.y += shiftY;
    splashStates[i].alpha -= 50;
  }
  // cout << endl;
}

void Player::render() {
  actByState();
  SDL_Rect tmp = playerSprites[pState][currentSpriteID / numFrame[pState]];

  // if (pState == 3 || pState == 4) {
  // printf("x = %d, y = %d, w = %d, h = %d \n", tmp.x, tmp.y, tmp.w, tmp.h);
  // printf("state = %d, currentSpriteID = %d\n", pState, currentSpriteID);
  // }

  pSpriteTextures[pState].renderCenter(gRenderer, pPosX, pPosY, P_SIZE, P_SIZE, 
                                       &tmp, pAngle);
  splash();
}

void Player::setState(PlayerState state) {pState = state;}
Player::PlayerState Player::getState() {return pState;}

int Player::getPosX() {return pPosX;}
int Player::getPosY() {return pPosY;}
