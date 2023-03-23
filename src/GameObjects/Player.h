#pragma once

#include "../constants.h"
#include "../util.h"
#include <deque>

class Player {
public:
  enum PlayerState {
    IDLE = 0,
    IDLE_TO_MOVE,
    MOVE,
    MOVE_TO_IDLE,
    DASH,
    TOTAL_STATES
  };

  // Player's texture base size (not player size, it varies!)
  static const int P_SIZE = 45;
  // Player's normal velocity
  static const int P_VEL = 4;
  // Player's dash velocity
  static const int DASH_VEL = 17;
  // Cooldown time (milisecond) until the next dash
  const float DASH_COOLDOWN = 500;

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

  void randSplash();
  void splash();

  void setState(PlayerState state);

  // Render our character
  void render();

  PlayerState getState();
  int getPosX();
  int getPosY();

private:
  struct splashState {
    SDL_Rect rect;
    float angle;
    int alpha;
  };

  vector<SDL_Rect> playerSprites[TOTAL_STATES];
  LTexture pSpriteTextures[TOTAL_STATES];
  LTexture pSplashUnder;

  int pPosX, pPosY;
  float pAngle;
  PlayerState pState;
  int currentSpriteID;
  float lastDashTime;
  int velocity;
  // For splashing effects
  deque<splashState> splashStates;
  // Control number of frame per sprite
  vector<int> numFrame = {1, 4, 1, 4, 1};
};
