#pragma once

#include "../constants.h"
#include "../util.h"
#include <deque>
#include <vector>

class Player {
public:
  enum PlayerState {
    IDLE = 0,
    IDLE_TO_MOVE,
    MOVE,
    MOVE_TO_IDLE,
    DASH,
    DIE,
    TOTAL_STATES
  };

  // Player's texture base size (not player size, it varies!)
  static const int P_SIZE = 45;
  // Player's normal velocity
  static const int P_VEL = 5;
  // Player's dash velocity
  static const int DASH_VEL = 20;
  // Cooldown time (milisecond) until the next dash
  const float DASH_COOLDOWN = 400;
  // After being hit, there's a lil time the player is protected
  const float INVINCIBLE_TIME = 2000;
  const int TOTAL_LIFE = 10;

  // Initialize Player with given position on the screen
  Player(int x = 0, int y = 0);
  ~Player();
  void loadMedia();
  // Set Player's Properties based on Key Press
  void handleKeyPress();

  // Player's Behavior base on State
  void actByState();
  void idle();
  void idleToMove();
  void move();
  void moveToIdle();
  void dash();

  void hit();
  bool isInvincible();
  // Render the die effect before the player's dead
  void die();
  bool isDead();

  void randSplash();
  void splash();

  void setState(PlayerState state);

  // Render our character
  void render();

  PlayerState getState();
  int getPosX();
  int getPosY();

  void refresh();

private:
  // Control number of frame per sprite
  const vector<int> numFrame = {1, 4, 1, 4, 1, 5};
  struct splashState {
    SDL_Rect rect;
    float angle;
    int alpha;
  };
  enum SFX_ID {
    HIT_SFX = 0,
    DIE_SFX,
    TOTAL_SFX
  };

  int currentSpriteID;
  vector<SDL_Rect> playerSprites[TOTAL_STATES];
  LTexture pSpriteTextures[TOTAL_STATES];

  int x, y;
  int life;
  float angle;
  PlayerState state;
  float lastDashTime, lastHit;

  // Control die effect
  bool dead;
  LTexture protectCirc;
  int sizeDisappear, alpha;

  // For splashing effects
  deque<splashState> splashStates;
  LTexture pSplashUnder;

  Mix_Chunk *sfx[TOTAL_SFX];
};
