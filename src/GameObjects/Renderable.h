#pragma once
#include "../constants.h"
#include "../util.h"

class Renderable {
public:
  Renderable();
  ~Renderable();

  virtual float getStartTime();
  virtual void render();
  virtual bool isRemovable();
  void loadMedia();
};
