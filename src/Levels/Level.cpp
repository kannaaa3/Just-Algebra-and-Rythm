#include "Level.h"

using namespace std;

// Some util function
int readFileIntoInt(const string &path) {
  ifstream input_file(path);
  if (!input_file.is_open()) {
    cerr << "Could not open the file - '" << path << "'" << endl;
    exit(EXIT_FAILURE);
  }
  return stoi(string((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>()));
}

bool isFloat(string myString) {
  std::istringstream iss(myString);
  float f;
  iss >> noskipws >> f; // noskipws considers leading whitespace invalid
  // Check the entire string was consumed and if either failbit or badbit is set
  return iss.eof() && !iss.fail();
}

Level::Level() {
  currentLevel = 0; // getNumLevel();
  numLevel = getNumLevel();
}

void Level::finish() {
  if (numLevel < currentLevel) {
    numLevel = currentLevel;
    setNumLevel();
  }
}

void Level::refresh() {
  enemyRender.clear();
  snkRender.clear();
  txtRender.clear();
}

Level::~Level() {}

void Level::loadMedia() {
  refresh();
  string path = "data/levels/Level" + to_string(currentLevel) + ".txt";
  cout << "Level Loadmedia: " << path << endl;
  string line;
  ifstream loadedFile(path);
  while (getline(loadedFile, line)) {
    istringstream iss(line);
    vector<string> s_args;
    vector<float> args;
    while (getline(iss, line, ' ')) {
      s_args.push_back(line);
      // cout << line << ", ";
    }
    if (s_args.size() == 0)
      continue;
    else if (isFloat(s_args[1])) {
      for (auto arg : s_args)
        args.push_back(stof(arg));
    }

    // cout << '\n';

    switch (stoi(s_args[0])) {
    case ENEMY: {
      // Moving Entity: x, y, w, h, vel, dir, angle, angleVel
      MovingEntity *me = new MovingEntity(
          {{(int)args[1], (int)args[2], (int)args[3], (int)args[4]},
           {args[5], args[6], args[7], args[8]}});
      Enemy::EnemyTimer t = {args[9], args[10], args[11], args[12]};
      Enemy *e = new Enemy(*me, t,
                           // EnemyTimer: start, idle, splash, normal
                           (int)args[13],         // alpha
                           (int)args[14],         // splash_extend
                           (Enemy::State)args[15] // state = IDLE = 0
      );
      enemyRender.push_back(*e);
      break;
    }
    case SQR_SNAKE: {
      //  startTime, {x, y}, dir, vel, size: 30-50, len: 10-20
      SqrSnake ss = SqrSnake(
          args[1], {static_cast<int>(args[2]), static_cast<int>(args[3])},
          args[4], args[5], args[6], args[7]);
      snkRender.push_back(ss);
      // enemyToRender.push_back(make_shared(ss)); PRenderable tmp =
      // enemyToRender.back();
      break;
    }
    case ALLY_TEXT: {
      replace(s_args[1].begin(), s_args[1].end(), '-', ' ');
      //
      BouncingText *bt = new BouncingText(
          s_args[1], stoi(s_args[2]), stoi(s_args[3]), stoi(s_args[4]),
          stof(s_args[5]), stof(s_args[6]), stof(s_args[7]));
      // cout <<"1 done" <<endl;
      txtRender.push_back(*bt);
      break;
    }
    }
    // cout << endl;
  }
  // TODO: Sort all the texture base on start time
  // sort(txtRender.begin(), txtRender.end(),
  //      [] (BouncingText a, BouncingText b) -> bool {
  //        return a.getStartTime() < b.getStartTime();
  //      });

  cout << "SDL_Error: " << SDL_GetError() << endl;
  for (auto &r : snkRender)
    r.loadMedia();
  for (auto &r : enemyRender)
    r.loadMedia();

  // NOTE: Music Loading
  gMusic = Mix_LoadMUS(("assets/global/sound/" + SONG_NAME[currentLevel]).c_str());
  if (gMusic == NULL) {
    printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
  }
}

void Level::setNumLevel() {
  ofstream num_level;
  num_level.open("data/level.txt", ofstream::trunc);
  num_level << numLevel;
  num_level.close();
}

int Level::getNumLevel() { return readFileIntoInt("data/level.txt"); }

void Level::handleKeyPress() {
  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  // If the current level is being pause
  if (gTimer.isPaused()) {
    if (currentKeyStates[SDL_SCANCODE_ESCAPE] && pausedTimer.getTicks() > 500) {
      gTimer.unpaused();
      pausedTimer.stop();
      Mix_ResumeMusic();
      cout << "UNPAUSE" << endl;
    } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {

    } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {

    } else if (currentKeyStates[SDL_SCANCODE_RETURN]) {
    }
  } else {
    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
      gTimer.paused();
      pausedTimer.start();
      Mix_PauseMusic();
      cout << "PAUSED" << endl;
    }
  }
}

void Level::run(Player* p) {
  while (snkRender.size() && snkRender.front().isRemovable())
    snkRender.pop_front();
  while (txtRender.size() && txtRender.front().isRemovable())
    txtRender.pop_front();
  while (enemyRender.size() && enemyRender.front().isRemovable())
    enemyRender.pop_front();
  for (int i = 0; i < snkRender.size(); i++) {
    if (snkRender[i].getStartTime() > gTimer.getTicks())
      break;
    snkRender[i].render();
  }
  for (int i = 0; i < txtRender.size(); i++) {
    if (txtRender[i].getStartTime() > gTimer.getTicks())
      break;
    txtRender[i].render();
  }
  for (int i = 0; i < enemyRender.size(); i++) {
    if (enemyRender[i].getStartTime() > gTimer.getTicks())
      break;
    enemyRender[i].render();
    if ((enemyRender[i].getState() == Enemy::SPLASH || enemyRender[i].getState() == Enemy::NORMAL) && enemyRender[i].checkCollision(p)) {
           cout << "Trung dan" << endl;
         }
  }
  // TODO: Check collision
}

void Level::playMusic() {
  Mix_PlayMusic(gMusic, 0);
}

bool Level::trackCompleted() {
  return Mix_PlayingMusic() == 0;
}
