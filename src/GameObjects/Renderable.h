#pragma once
#include "../constants.h"
#include "../util.h"

class Renderable {
public:
  Renderable();
  ~Renderable();

  virtual float startTime();
  virtual void render();
  virtual bool isRemovable();
};
