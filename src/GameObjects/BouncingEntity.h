#pragma once
#include "../constants.h"
#include "../util.h"
#include "Entity.h"
#include "Renderable.h"

// The entity will bounce by going down and back
// Delay a small time before the next bounce
class BouncingEntity : public Entity {
  enum Direction { UP = 0, DOWN };

public:
  BouncingEntity(SDL_Rect r = {SCREEN_WIDTH / 2, 0, 0, 0}, int distance = 50,
                 float time = 300);
  ~BouncingEntity();

  void move();

private:
  const float B_GAP = 20;
  const float B_DELAY = 100;
  int posY[2]; // posY[0] <= y <= posY[1]
  int dir;
  float bTime;
  float bLast;
};

class BouncingText : public BouncingEntity, public Renderable {
public:
  enum State {
    IDLE = 0,
    NORMAL,
    DISSAPPEAR
  };
  BouncingText(string text, int fontType = 0, int x = SCREEN_WIDTH / 2, int y = 200,
               float startTime = 0, float appearTime = 5000, float timePerBounce = 300, 
               SDL_Color color = ALLY_BLUE);
  ~BouncingText();
  void loadMedia();

  void actByState();

  void render();
  bool isRemovable();

  float getStartTime();

  // NOTE: FIXED WHEN CHANGE TO REFERENCE
  LTexture *textTexture;
  float startTime, endTime;
  State state;
  SDL_Rect renderQuad;
  bool removable;
private:
};
