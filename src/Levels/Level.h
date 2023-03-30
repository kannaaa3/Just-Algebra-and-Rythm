#pragma once

#include "../util.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/SqrSnake.h"
#include "../GameObjects/Renderable.h"
#include "../GameObjects/BouncingEntity.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
// typedef shared_ptr<Renderable> PRenderable;

class Level {
public:
  enum RenderType {
    ENEMY = 0,
    SQR_SNAKE,
    ALLY_TEXT,
    TOTAL_RENDER_TYPE
  };
  Level();
  ~Level();

  void loadMedia(string path);
  // Save the level number
  void setNumLevel();
  int getNumLevel();

  deque<Renderable> enemyToRender;
  deque<Enemy> enemyRender;
  deque<SqrSnake> snkRender;
  deque<BouncingText> txtRender;

  vector<string> txtVRender;
  vector<int> fontID;

private:
  int numLevel = 12;
};

