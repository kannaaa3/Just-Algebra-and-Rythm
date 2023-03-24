#include "Player.h"
#include <iostream> // TODO: Delete
using namespace std;

Player::Player(int x, int y) {
  this->x = x;
  this->y = y;
  this->angle = 0;
  this->state = IDLE;
  currentSpriteID = 0;
  lastDashTime = -DASH_COOLDOWN;
  // Splash
}

Player::~Player() {}

void Player::loadMedia() {
  SDL_Rect tmpRect = {0, 0, P_SIZE, P_SIZE};

  string prefix = "assets/GameObjects/img/";
  if (!pSpriteTextures[IDLE].loadFromFile(gRenderer, prefix + "PlayerIdle.png"))
    printf("Failed to load PlayerIdle texture!\n");

  if (!pSpriteTextures[IDLE_TO_MOVE].loadFromFile(
          gRenderer, prefix + "PlayerIdleToMoveSprites.png"))
    printf("Failed to load PlayerIdleToMove texture!\n");

  if (!pSpriteTextures[MOVE].loadFromFile(gRenderer, prefix + "PlayerMove.png"))
    printf("Failed to load PlayerMove texture!\n");

  if (!pSpriteTextures[MOVE_TO_IDLE].loadFromFile(
          gRenderer, prefix + "PlayerMoveToIdleSprites.png"))
    printf("Failed to load PlayerMoveToIdle texture!\n");

  if (!pSpriteTextures[DASH].loadFromFile(gRenderer,
                                          prefix + "PlayerDashSprites.png"))
    printf("Failed to load PlayerDash texture!\n");

  if (!pSplashUnder.loadFromFile(gRenderer, prefix + "PlayerSplash.png"))
    printf("Failed to load PlayerIdle texture!\n");

  for (int i = IDLE; i != TOTAL_STATES; i++) {
    for (int j = 0;
         j * pSpriteTextures[i].getHeight() < pSpriteTextures[i].getWidth();
         j++) {
      tmpRect.x = P_SIZE * j;
      playerSprites[i].push_back(tmpRect);
    }
  }
}

void Player::randSplash() {
  SDL_Rect tmpRect = {randomNumber(x - P_SIZE / 2 + 15, x + P_SIZE / 2 - 15),
                      randomNumber(y - P_SIZE / 2 + 15, y + P_SIZE / 2 - 15),
                      15, 15};
  splashState tmpSS;
  tmpSS.rect = tmpRect;
  tmpSS.alpha = 180;
  tmpSS.angle = fmod(angle + 180, 360);
  splashStates.push_back(tmpSS);
}

void Player::handleKeyPress() {
  // if dashing, not handle any key press
  if (state == DASH)
    return;

  Uint8 keyPress[] = {SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_LEFT,
                      SDL_SCANCODE_DOWN};
  float angleKeyPress[] = {0, 90, 180, 270};

  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  for (int i = 0; i <= 4; i++) {
    // if a key was press, or the player MOVE
    if (i != 4 && currentKeyStates[keyPress[i]]) {
      // set angle
      angle = angleKeyPress[i];
      if (currentKeyStates[keyPress[(i + 1) % 4]])
        angle += 45.0;
      else if (currentKeyStates[keyPress[(i + 3) % 4]])
        angle -= 45.0;
      if (angle < 0)
        angle += 360.0;

      // NOTE: Add splash twice
      randSplash();
      randSplash();
      /////////////////////////////////////////////////////////////////////////////

      switch (state) {
      case IDLE:
        // So that it could add to 0
        currentSpriteID = -1;
        state = IDLE_TO_MOVE;
        break;
      case MOVE_TO_IDLE:
        // So that it could add to 0
        currentSpriteID = -1;
        state = IDLE_TO_MOVE;
        break;
      }
      // Stop iterating
      break;
    }
    // none of the key was pressed
    if (i == 4) {
      switch (state) {
      case MOVE:
        currentSpriteID = -1;
        state = MOVE_TO_IDLE;
        break;
      case IDLE_TO_MOVE:
        currentSpriteID = -1;
        state = MOVE_TO_IDLE;
        break;
      }
    }
  }

  // If player dash
  if (currentKeyStates[SDL_SCANCODE_SPACE] &&
      DASH_COOLDOWN < gTimer.getTicks() - lastDashTime) {
    // cout << "Time minus = " << gTimer.getTicks() - lastDashTime << endl;
    currentSpriteID = -1;
    setState(DASH);
  }
}

void Player::actByState() {
  switch (state) {
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
  if (currentSpriteID + 1 >=
      numFrame[IDLE_TO_MOVE] * playerSprites[IDLE_TO_MOVE].size()) {
    // TODO:  change to move
    setState(MOVE);
    // cout << "Change to MOVE\n";
    currentSpriteID = 0;
  } else {
    currentSpriteID++;
  }

  int shiftX = P_VEL * cos(angle / 180 * M_PI);
  int shiftY = -P_VEL * sin(angle / 180 * M_PI);

  // If not go out of screen
  if (not(x + shiftX < P_SIZE / 2 + 1 ||
          x + shiftX + P_SIZE / 2 > SCREEN_WIDTH ||
          y + shiftY < P_SIZE / 2 + 1 ||
          y + shiftY + P_SIZE / 2 > SCREEN_HEIGHT)) {
    x += shiftX;
    y += shiftY;
  }
}

void Player::move() {
  if (currentSpriteID >= playerSprites[MOVE].size()) {
    currentSpriteID = 0;
  }

  // Move the player
  int shiftX = P_VEL * cos(angle / 180 * M_PI);
  int shiftY = -P_VEL * sin(angle / 180 * M_PI);

  // If not go out of screen
  if (not(x + shiftX < P_SIZE / 2 + 1 ||
          x + shiftX + P_SIZE / 2 > SCREEN_WIDTH ||
          y + shiftY < P_SIZE / 2 + 1 ||
          y + shiftY + P_SIZE / 2 > SCREEN_HEIGHT)) {
    x += shiftX;
    y += shiftY;
  }
}

void Player::moveToIdle() {
  if (currentSpriteID + 1 >=
      numFrame[MOVE_TO_IDLE] * playerSprites[MOVE_TO_IDLE].size()) {
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
    // NOTE: If not, it will still move even though no key press
    handleKeyPress();
  } else {
    // TODO: Continue dashing
    currentSpriteID++;

    // Move the player
    int shiftX = DASH_VEL * cos(angle / 180 * M_PI);
    int shiftY = -DASH_VEL * sin(angle / 180 * M_PI);

    // If not go out of screen
    if (not(x + shiftX < P_SIZE / 2 + 1 ||
            x + shiftX + P_SIZE / 2 > SCREEN_WIDTH ||
            y + shiftY < P_SIZE / 2 + 1 ||
            y + shiftY + P_SIZE / 2 > SCREEN_HEIGHT)) {
      x += shiftX;
      y += shiftY;
    }
  }
}

void Player::splash() {
  while (splashStates.size() &&
         (splashStates.front().rect.w < 1 || splashStates.front().alpha <= 0))
    splashStates.pop_front();

  for (int i = 0; i < splashStates.size(); i++) {
    if (splashStates.front().alpha <= 0)
      continue;
    int shiftX = 2 * cos(splashStates[i].angle / 180 * M_PI);
    int shiftY = -2 * sin(splashStates[i].angle / 180 * M_PI);
    SDL_Rect spr = {0, 0, splashStates[i].rect.w, splashStates[i].rect.h};

    // cout << "alpha = "  << splashStates.front().alpha <<", w = " << spr.w
    // <<", h =  " << spr.h << endl;

    pSplashUnder.setAlpha(splashStates[i].alpha);
    pSplashUnder.renderCenter(gRenderer, splashStates[i].rect.x,
                              splashStates[i].rect.y, spr.w, spr.h, &spr,
                              splashStates[i].angle);

    splashStates[i].rect.w -= 1;
    splashStates[i].rect.h -= 1;
    splashStates[i].rect.x += shiftX;
    splashStates[i].rect.y += shiftY;
    splashStates[i].alpha -= 15;
  }
}

void Player::render() {
  actByState();
  SDL_Rect tmp = playerSprites[state][currentSpriteID / numFrame[state]];

  // if (pState == 3 || pState == 4) {
  // printf("x = %d, y = %d, w = %d, h = %d \n", tmp.x, tmp.y, tmp.w, tmp.h);
  // printf("state = %d, currentSpriteID = %d\n", pState, currentSpriteID);
  // }

  // Splash under Texture
  splash();
  pSpriteTextures[state].renderCenter(gRenderer, x, y, P_SIZE, P_SIZE, &tmp,
                                      angle);
}

void Player::setState(PlayerState state) { this->state = state; }
Player::PlayerState Player::getState() { return state; }

int Player::getPosX() { return x; }
int Player::getPosY() { return y; }
