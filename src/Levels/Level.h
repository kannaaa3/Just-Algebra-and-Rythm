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

class DisplayTrackName {
public:
  DisplayTrackName(string name = "", string author = "");
  ~DisplayTrackName();

  void render();

  bool refresh(string name = "", string author = "");

  bool isRemovable();

private:
  const float TIME = 500, APPEAR_TIME = 2000;
  enum textureID {
    NAME = 0,
    BY,
    AUTHOR,
    TOTAL_TEXT
  };
  string name, author;
  LTexture *text[TOTAL_TEXT];
  TTF_Font *font[TOTAL_TEXT];

  const vector<SDL_Point> pos = {{1873, 816}, {1831, 937}, {1831, 995}};
};

class Level {
public:
  enum RenderType { ENEMY = 0, SQR_SNAKE, ALLY_TEXT, TOTAL_RENDER_TYPE };

  Level();
  ~Level();
  void finish();

  void loadMedia();
  // Save the level number
  void setNumLevel();
  int getNumLevel();

  void playMusic();
  void refresh();

  void handleKeyPress(const SDL_Event e);
  void run(Player* p);

  void pause();
  void unpause();

  bool trackCompleted();
  void displayTrackName();

private:
  const string SONG_FILE[3] = { "cYsmix - Peer Gynt.wav", "Crazy_Pharaoh_Fred_Eddy_Remix.wav", "Fred Eddy - Vietnam.wav"};
  const string SONG_NAME[3] = { "PEER GYNT", "CRAZY PHARAOH", "VIETNAM"};
  const string SONG_AUTHOR[3] = { "eYsmix", "Fred Eddy", "Fred Eddy"};
  int numLevel;
  int currentLevel;

  deque<PRenderable> enemyToRender;
  deque<Enemy> enemyRender;
  deque<SqrSnake> snkRender;
  deque<BouncingText> txtRender;

  LTimer pausedTimer;
  DisplayTrackName *trackname;
};

class PauseScreen {
public:
  PauseScreen();
  ~PauseScreen();
  void render();
private:
  enum textureID {
    PAUSED = 0,
    CONTINUE,
    QUIT,
    TOTAL_TEXT
  };
  const vector<SDL_Point> pos = {{1873, 816}, {1831, 937}, {1831, 995}};
  LTexture *blackBG;
};
