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

class ProgressBar {
public:
  ProgressBar(int duration = 3000);
  ~ProgressBar();

  void render();
  void refresh(int duration = 3000);
private:
  // X, Y: upper right corner
  const int W = 813, H = 37, X = 1367, Y = 22;
  const int SQR_SIZE = 15;
  const float TIME_APPEAR = 1200;
  LTexture *barTexture;
  //center of sqr
  int sqr_x, sqr_y;
  int duration;
};

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

class PauseScreen {
public:
  PauseScreen();
  ~PauseScreen();

  int handleKeyPress(const SDL_Event e);
  void render();

  void refresh();
private:
  enum textureID {
    CONTINUE = 0,
    QUIT,
    PAUSED,
    TOTAL_TEXT
  };
  const vector<SDL_Point> pos = {{848, 580}, {1090, 580}, {960, 450}};
  LTexture *blackBG, *selectedEffect;
  LTexture *text[TOTAL_TEXT], *blue_text[TOTAL_TEXT - 1];

  enum sfxID {
    NAVIGATE = 0,
    SELECT,
    TOTAL_SFX
  };
  Mix_Chunk *sfx[2];
  int selected_section;
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

  int handleKeyPress(const SDL_Event e);
  // Render function
  void run(Player* p);

  void pause();
  void unpause();
  bool isPaused();

  bool trackCompleted();
  void displayTrackName();

private:
  // struct Song {
  //   string file;
  //   string name;
  //   string author;
  //   int duration;
  // } song[3];
  const string SONG_FILE[3] = { "cYsmix - Peer Gynt.wav", "Crazy_Pharaoh_Fred_Eddy_Remix.wav", "Fred Eddy - Vietnam.wav"};
  const string SONG_NAME[3] = { "PEER GYNT", "CRAZY PHARAOH", "VIETNAM"};
  const string SONG_AUTHOR[3] = { "eYsmix", "Fred Eddy", "Fred Eddy"};
  const int DURATION[3] = {141479, 1, 1};
  int numLevel;
  int currentLevel;

  deque<PRenderable> enemyToRender;
  deque<Enemy> enemyRender;
  deque<SqrSnake> snkRender;
  deque<BouncingText> txtRender;

  LTimer pausedTimer;
  DisplayTrackName *trackName;
  PauseScreen *pauseScreen;
  ProgressBar *progressBar;
};
