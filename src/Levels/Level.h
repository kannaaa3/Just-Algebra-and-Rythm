#pragma once

// #include "../util.h"
// #include "../GameObjects/MovingEntity.h"

#include <iostream>
using namespace std;

class Level {
public:
  enum EnemyType {
    ENEMY = 0,
    SQR_SNAKE,
    TOTAL_ENEMY_TYPE
  };
  Level();
  ~Level();

  void loadMedia(string path);
  // Save the level number
  void setNumLevel();
  int getNumLevel();

private:
  int numLevel = 12;
};

