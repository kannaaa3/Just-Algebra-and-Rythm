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
  trackname = new DisplayTrackName(SONG_NAME[currentLevel], SONG_AUTHOR[currentLevel]);
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
  pausedTimer.stop();
  pausedTimer.start();
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
  gMusic = Mix_LoadMUS(("assets/global/sound/" + SONG_FILE[currentLevel]).c_str());
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

void Level::pause() {
  cout << "PAUSE" << endl;
  gTimer.paused();
  pausedTimer.stop();
  pausedTimer.start();
  Mix_PauseMusic();
}

void Level::unpause() {
  gTimer.unpaused();
  pausedTimer.stop();
  pausedTimer.start();
  Mix_ResumeMusic();
  cout << "UNPAUSE" << endl;
}

void Level::handleKeyPress(const SDL_Event e) {
  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  // If the current level is being pause
  if (gTimer.isPaused()) {
    if ((currentKeyStates[SDL_SCANCODE_ESCAPE] || e.key.keysym.sym == SDLK_ESCAPE) && pausedTimer.getTicks() > 500) {
      unpause();
    } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {

    } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {

    } else if (currentKeyStates[SDL_SCANCODE_RETURN]) {
    }
  } else {
    if (e.key.keysym.sym == SDLK_ESCAPE && pausedTimer.getTicks() > 500) {
      pause();
    }
  }
}

void Level::run(Player* p) {
  displayTrackName();
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
  // TODO: Check collision
    if ((!p->isDead()) && ( enemyRender[i].getState() == Enemy::SPLASH || enemyRender[i].getState() == Enemy::NORMAL) && enemyRender[i].checkCollision(p)) {
           cout << "Trung dan" << endl;
            p->hit();
         }
  }
}

void Level::playMusic() {
  Mix_PlayMusic(gMusic, 0);
}

bool Level::trackCompleted() {
  return Mix_PlayingMusic() == 0;
}

void Level::displayTrackName() {
  if (!trackname->isRemovable())
    trackname->render();
}

DisplayTrackName::DisplayTrackName(string name, string author) {
  for(int i = 0; i != TOTAL_TEXT; i++) {
    text[i] = new LTexture();
  }
  
  font[NAME] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 120);
  font[BY] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 50);
  font[AUTHOR] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 50);
  
  text[BY]->loadFromRenderedText(gRenderer, font[BY], "by", ALLY_BLUE);
  if(!refresh(name, author)) {
    printf("Failed to load DisplayTrackName: name = %s, author = %s! SDL_Error: %s\n", name.c_str(), author.c_str(), SDL_GetError());
  }
}

DisplayTrackName::~DisplayTrackName() {
  for(int i = 0; i != TOTAL_TEXT; i++) {
    text[i]->free();
    TTF_CloseFont(font[i]);
    font[i] = NULL;
  }
}

void DisplayTrackName::render() {
  float ratio, time = gTimer.getTicks();
  if (time <= TIME + APPEAR_TIME) {
    ratio = time/TIME;
    ratio = min((float) 1.0, ratio);
  } else {
    ratio = 1.0 - (time - TIME - APPEAR_TIME) / TIME;
    ratio = max((float) 0.0, ratio);
  }

  for(int i = 0; i != TOTAL_TEXT; i++) {
    int w = ratio * ((float) text[i]->getWidth());
    SDL_Rect r = {(int) (text[i]->getWidth() - w) , 0, w, text[i]->getHeight()};
    text[i]->render(gRenderer, pos[i].x - w, pos[i].y, &r);
    cout << SDL_GetError();
  }
}

bool DisplayTrackName::refresh(string name, string author) {
  bool success = true;
  if (!text[NAME]->loadFromRenderedText(gRenderer, font[NAME], name, WHITE)) {
    success = false;
    printf("Failed to load Name: %s text texture! SDL_Error: %s", name.c_str(), SDL_GetError());
  }
  if (!text[AUTHOR]->loadFromRenderedText(gRenderer, font[AUTHOR], author, ALLY_BLUE)) {
    success = false;
    printf("Failed to load Author: %s text texture! SDL_Error: %s", author.c_str(), SDL_GetError());
  }
  return success;
}

bool DisplayTrackName::isRemovable() {
  return gTimer.getTicks() > TIME + APPEAR_TIME + TIME;
}

PauseScreen::PauseScreen() {
  blackBG = new LTexture();
  if (!blackBG->loadFromFile(gRenderer, "assets/global/img/blackBG.png")) {
    printf("Failed to load blackBG texture! SDL_Error: %s\n", SDL_GetError());
  } else {
    blackBG->setDimension(SCREEN_WIDTH, SCREEN_HEIGHT);
  }
}

PauseScreen::~PauseScreen() {
  blackBG->free();
}

void PauseScreen::render() {
  SDL_SetRenderDrawColor(gRenderer, ENEMY_PINK.r, ENEMY_PINK.g, ENEMY_PINK.b, 0xFF);
  SDL_Rect tmp = {0, 0, 338, 411};
  SDL_RenderFillRect(gRenderer, &tmp);
  blackBG->render(gRenderer, 0, 0);
}
