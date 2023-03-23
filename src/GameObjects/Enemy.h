#pragma once 

#include "../util.h"
#include "Player.h"
#include "../constants.h"
#include "MovingEntity.h"


// By Default, an Enemy is a rectangle
class Enemy  {
public:
  enum State {
    IDLE = 0,
    SPLASH,
    NORMAL,
    DISAPPEAR,
    TOTAL_STATES
  };

  struct EnemyTimer
  {
    float start = 0;
    float idle = 1500;
    float splash = 600;
    float normal = 2000;
  };

  Enemy(MovingEntity me = MovingEntity(),
        EnemyTimer t = {0, 1500, 600, 200}, 
       int alpha = 0, int splash_extend = 50, State s = IDLE);
  ~Enemy();

  // Action based on current state
  void actByState();
  // Check whether it collide . TODO: set global Player
  bool checkCollision(Player* p);
  //Display the entity on the screen
  void render();
  bool isRemovable();

  // Setter and Getter
  void setState (State s);
  State getState();

  void setStartTime (float t);
  float getStartTime ();

  void loadMedia();

  LTexture mTexture[TOTAL_STATES];
  // NOTE: Remember to loadMedia AFTER all properties have been set
  void idle();
  void splash();
  void normal();
  void disappear();

private:
  // Control the enemy's pos, shape and movement.
  MovingEntity shape;
  // Control state's presence by time. By default,
  // Start time of CURRENT state
  // normalTime = 4s
  // splashTime = 0.3s
  // idleTime = 1s
  EnemyTimer time;
  // Current state of entity
  State mState;
  // size extend when splash
  int mSplashExtend;

  bool removable;

  /* control opacity of enemy */
  const int MAX_IDLE_ALPHA = 100;
  int mAlpha;
  int numIncAlph;
};
