#pragma once

#include "../GameObjects/BouncingEntity.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Renderable.h"
#include "../GameObjects/SqrSnake.h"
#include "../util.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
typedef shared_ptr<Renderable> PRenderable;

class Level {
public:
  enum RenderType { ENEMY = 0, SQR_SNAKE, ALLY_TEXT, TOTAL_RENDER_TYPE };

  Level();
  ~Level();
  void finish();

  void loadMedia(string path);
  // Save the level number
  void setNumLevel();
  int getNumLevel();

  void handleKeyPress();
  void run(Player* p);

private:
  int numLevel;
  int currentLevel;

  deque<PRenderable> enemyToRender;
  deque<Enemy> enemyRender;
  deque<SqrSnake> snkRender;
  deque<BouncingText> txtRender;

  LTimer pausedTimer;

  const vector<string> SONG_NAME = {"", "cYsmix - Peer Gynt.wav", ""};
};
