#include "../util.h"
#include "Player.h"
#include "../constants.h"

class MovingEntity {
public:
    enum State {
    IDLE = 0,
    SPLASH,
    NORMAL,
    DISAPPEAR,
    TOTAL_STATES
  };

  MovingEntity(int x = SCREEN_WIDTH / 2, int y = SCREEN_HEIGHT / 2, int w = 0, int h = 0, 
               float angle = 0, float angleVel = 0, int vel = 0, float dir = 0,
               float idle_time = 1500, float splash_time = 600, float normal_time = 2000, 
               int alpha = 0, int splash_extend = 20, State s = IDLE);
  ~MovingEntity();

  // Action based on current state
  void actByState();
  // Check whether it collide . TODO: set global Player
  bool checkCollision(Player* p);
  //Display the entity on the screen
  void render();
  bool isRemovable();

  // Setter and Getter
  void setPosition (int x, int y);
  pair<int, int> getPosition();

  void setDimension (int w, int h);
  pair<int, int> getDimension();

  void setAngle (float angle);
  float getAngle();

  void setAngleVel (float angleVel);
  float getAngleVel();

  void setVel (int vel);
  int getVel();

  void setDirection (float dir);
  float getDirection();

  void setState (State s);
  State getState();

  void setStartTime (float t);

  // Set idle time by milisecond
  void setIdleTime(float t);
  // Set splash time by milisecond
  void setSplashTime(float t);
  // Set normal time by milisecond
  void setNormalTime(float t);

  void loadMedia();

  LTexture mTexture[TOTAL_STATES];
  // NOTE: Remember to loadMedia AFTER all properties have been set
  void idle();
  void splash();
  void normal();
  void disappear();

private:
  // Position of center of texture. By default, this lies on the center of screen
  // mPosX = SCREEN_WIDTH / 2; mPosY = SCREEN_HEIGHT/2
  int mPosX, mPosY;
  // Dimension of entity. By default, is zero.
  int mWidth, mHeight;
  // Spinning State of entity. By default, is zero.
  float mAngle, mAngleVel;
  // Moving velocity. By default, is zero.
  int mVel;
  // Angle in (trigonometric) unit circle, control the Moving direction of entity
  float mDir;
  // Control state's presence by time. By default,
  // normalTime = 4s
  // splashTime = 0.3s
  // idleTime = 1s
  float normalTime, splashTime, idleTime;
  // Current state of entity
  State mState;
  // Start time of CURRENT state
  float startTime;
  int numIncAlph;
  // size extend when splash
  int mSplashExtend;

  const int MAX_IDLE_ALPHA = 100;
  int mAlpha;


  bool removable;
};
