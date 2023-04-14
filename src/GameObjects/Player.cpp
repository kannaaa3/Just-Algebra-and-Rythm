#include "Player.h"

Player::Player(int x, int y) {
  this->x = x;
  this->y = y;
  this->angle = 0;
  this->state = IDLE;
  currentSpriteID = 0;
  lastDashTime = -DASH_COOLDOWN;
  lastHit = -INVINCIBLE_TIME;
  life = TOTAL_LIFE;
  dead = false;
}

Player::~Player() {
  for(int i = 0; i != TOTAL_STATES; i++) {
    pSpriteTextures[i].free();
  }
  playerSprites->clear();
  splashStates.clear();
  pSplashUnder.free();
}

void Player::loadMedia() {
  // Load Texture
  SDL_Rect tmpRect = {0, 0, P_SIZE, P_SIZE};
  string prefix_path = "assets/GameObjects/img/";
  const string textureName[] = {"PlayerIdle.png", "PlayerIdleToMoveSprites.png", "PlayerMove.png", "PlayerMoveToIdleSprites.png", "PlayerDashSprites.png", "PlayerDie.png"};

  for(int i = 0; i != TOTAL_STATES; i++) {
    if (!pSpriteTextures[i].loadFromFile(gRenderer, prefix_path + textureName[i]))
      printf("Failed to load %s texture!\n", textureName[i].c_str());
  }
  for (int i = IDLE; i != TOTAL_STATES; i++) {
    for (int j = 0;
         j * pSpriteTextures[i].getHeight() < pSpriteTextures[i].getWidth();
         j++) {
      tmpRect.x = P_SIZE * j;
      playerSprites[i].push_back(tmpRect);
    }
  }
  if (!pSplashUnder.loadFromFile(gRenderer, prefix_path + "PlayerSplash.png"))
    printf("Failed to load PlayerSplash texture!\n");
  if (!protectCirc.loadFromFile(gRenderer, prefix_path + "PlayerProtect.png"))
    printf("Failed to load PlayerProtect texture!\n");
  // Load SFX
  prefix_path = "assets/GameObjects/sound/";
  const string sfxName[] = {"player_hit.wav", "player_die.wav"};
  for(int i = 0; i != TOTAL_SFX; i++) {
    sfx[i] = Mix_LoadWAV((prefix_path + sfxName[i]).c_str());
    if (NULL == sfx[i])
      printf("Failed to load %s sound! SDL_Error: %s\n", sfxName[i].c_str(), SDL_GetError());
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
  if (state == DASH || gTimer.isPaused() || isDead()) {
    return;
  }

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
  if (gTimer.isPaused() || isDead())
    return;
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
  case DIE:
    die();
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

  int shiftX = P_VEL * cos(toRad(angle));
  int shiftY = -P_VEL * sin(toRad(angle));

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
  int shiftX = P_VEL * cos(toRad(angle));
  int shiftY = -P_VEL * sin(toRad(angle));

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
    // NOTE:If not do this, it will still move even though no key press
    handleKeyPress();
  } else {
    // TODO: Continue dashing
    currentSpriteID++;

    // Move the player
    int shiftX = DASH_VEL * cos(toRad(angle));
    int shiftY = -DASH_VEL * sin(toRad(angle));

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

void Player::hit() {
  if (life == 0) return;
  Mix_PlayChannel(-1, sfx[HIT_SFX], 0);
  lastHit = gTimer.getTicks();
  life--;
  // If the player just die
  if (life == 0) {
    gTimer.stop();
    Mix_PlayChannel(-1, sfx[DIE_SFX], 0);
    state = DIE;
    angle = 0;
    alpha = 0xFF;
    sizeDisappear = P_SIZE;
  }
}

void Player::die() {
  // TODO: render Die: Spin until disappear
  angle += 3.0;
  alpha = max(0, alpha - 15);
  pSpriteTextures[DIE].setAlpha(alpha);
  sizeDisappear--;
  // After rendering the Die effect, set Dead = true
  if (sizeDisappear == 0 || alpha == 0) {
    dead = true;
  }
}

bool Player::isDead() {
  return dead;
}

void Player::splash() {
  while (splashStates.size() &&
         (splashStates.front().rect.w < 1 || splashStates.front().alpha <= 0))
    splashStates.pop_front();

  for (int i = 0; i < splashStates.size(); i++) {
    if (splashStates.front().alpha <= 0)
      continue;
    int shiftX = 2 * cos(toRad(splashStates[i].angle));
    int shiftY = -2 * sin(toRad(splashStates[i].angle));
    SDL_Rect spr = {0, 0, splashStates[i].rect.w, splashStates[i].rect.h};

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

bool Player::isInvincible() {
  return gTimer.getTicks() < lastHit + INVINCIBLE_TIME || state == DASH;
}

void Player::render() {
  actByState();
  // cout << "Player " << state << endl;
  if (life == 0) {
    // cout << "life == 0" << endl;
    pSpriteTextures[DIE].setDimension(sizeDisappear, sizeDisappear);
    pSpriteTextures[DIE].renderCenter(gRenderer, x, y, sizeDisappear, sizeDisappear, NULL, angle);
    // cout << "Done render DIE" << endl;
    return;
  }
  // Invincible Mode within 2s after Being hit
  if (isInvincible()) {
    protectCirc.renderCenter(gRenderer, x, y, 64, 64);
    DrawCircle(gRenderer, x, y, 32);
  }

  if (gTimer.getTicks() < lastHit + INVINCIBLE_TIME) {
    SDL_SetRenderDrawColor(gRenderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
    SDL_Rect r = {x - P_SIZE/2, y + P_SIZE/2 + 15, P_SIZE * life / TOTAL_LIFE, 2};
    SDL_RenderFillRect(gRenderer, &r);
  }

  // Splash under Texture
  splash();
  // Render player's texture
  SDL_Rect tmp = playerSprites[state][currentSpriteID / numFrame[state]];
  pSpriteTextures[state].renderCenter(gRenderer, x, y, P_SIZE, P_SIZE, &tmp, angle);
}

void Player::setState(PlayerState state) { this->state = state; }
Player::PlayerState Player::getState() { return state; }

int Player::getPosX() { return x; }
int Player::getPosY() { return y; }

void Player::refresh() {
  angle = 0;
  state = IDLE;
  currentSpriteID = 0;
  lastDashTime = -DASH_COOLDOWN;
  lastHit = -INVINCIBLE_TIME;
  life = TOTAL_LIFE;
  dead = false;
}
