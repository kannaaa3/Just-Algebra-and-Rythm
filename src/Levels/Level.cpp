#include "Level.h"

using namespace std;



ProgressBar::ProgressBar(int duration) {
  barTexture = new LTexture();
  if (!barTexture->loadFromFile(gRenderer, "assets/global/img/progressBar.png")) {
    printf("Failed to load progress bar texture! SDL_Error: %s\n", SDL_GetError());
  }
  sqr_y = 40;
  sqr_x = 573;
  this->duration = duration;
}

ProgressBar::~ProgressBar() {
  barTexture->free();
}

void ProgressBar::render() {
  float ratio, time = gTimer.getTicks();
  ratio = (time > TIME_APPEAR) ? 1.0 : (time / TIME_APPEAR);

  int w = ratio * ((float) W);
  SDL_Rect r = {(int) W - w, 0, w, H};
  barTexture->render(gRenderer, X - w, Y, &r);

  if (time <= TIME_APPEAR) return;

  int shiftByDuration = (time - TIME_APPEAR < duration) ? (W - H) * (time - TIME_APPEAR) / duration : 1920;

  SDL_SetRenderDrawColor(gRenderer, DARK_GREEN.r, DARK_GREEN.g, DARK_GREEN.b, 0xFF);
  r = {sqr_x - SQR_SIZE/2 - 5 + shiftByDuration,  sqr_y - SQR_SIZE/2 + 5, SQR_SIZE, SQR_SIZE};
  SDL_RenderFillRect(gRenderer, &r);

  SDL_SetRenderDrawColor(gRenderer, ALLY_BLUE.r, ALLY_BLUE.g, ALLY_BLUE.b, 0xFF);
  r = {sqr_x - SQR_SIZE/2 + shiftByDuration,  sqr_y - SQR_SIZE/2, SQR_SIZE, SQR_SIZE};
  SDL_RenderFillRect(gRenderer, &r);
}

void ProgressBar::refresh(int duration) {
  sqr_x = 573; // X - W/2 + H/2
  this->duration = duration;
}

Level::Level() {
  //TODO: FIX
  currentLevel = getNumLevel();
  numLevel = getNumLevel();
  trackName = new DisplayTrackName(SONG[currentLevel].name, SONG[currentLevel].author);
  pauseScreen = new PauseScreen();
  progressBar = new ProgressBar(SONG[currentLevel].duration);
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
  progressBar->refresh(SONG[currentLevel].duration);
  trackName->refresh(SONG[currentLevel].name, SONG[currentLevel].author);
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
}

void Level::setNumLevel() {
  ofstream num_level;
  num_level.open("data/level.txt", ofstream::trunc);
  num_level << numLevel;
  num_level.close();
}

void Level::setCurrentLevel(int level) {
  currentLevel = level;
}

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

bool Level::isPaused() {
  return gTimer.isPaused();
}

int Level::handleKeyPress(const SDL_Event e) {
  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  // If the current level is being pause
  if (isPaused()) {
    // if ((currentKeyStates[SDL_SCANCODE_ESCAPE] || e.key.keysym.sym == SDLK_ESCAPE) && pausedTimer.getTicks() > 500) {
    //   unpause();
    // } else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
    //
    // } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
    //
    // } else if (currentKeyStates[SDL_SCANCODE_RETURN]) {
    // }
    int nextState = pauseScreen->handleKeyPress(e);
    // INGAME
    if (nextState == 0) unpause();
    else if (nextState == 6) {
      return 6;
    }
  } else {
    if (SDLK_ESCAPE == e.key.keysym.sym && pausedTimer.getTicks() > 500) {
      pause();
    }
  }
  return 0;
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
    // if ((!p->isDead()) && ( snkRender[i].getState() == Enemy::SPLASH || snkRender[i].getState() == Enemy::NORMAL) && snkRender[i].checkCollision(p)) {
    //   cout << "Trung dan" << endl;
    //   p->hit();
    // }
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
  progressBar->render();
  displayTrackName();
  if (isPaused()) pauseScreen->render();
}

void Level::playMusic() {
  Mix_PlayMusic(SONG[currentLevel].music, 0);
}

bool Level::trackCompleted() {
  return Mix_PlayingMusic() == 0;
}

void Level::displayTrackName() {
  if (!trackName->isRemovable())
    trackName->render();
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
  selectedEffect = new LTexture();
  if (!selectedEffect->loadFromFile(gRenderer, "assets/global/img/selectedTextEffect.png"))
      printf("Failed to load selectedTextEffect texture! SDL_Error: %s\n", SDL_GetError());
  
  blackBG = new LTexture();
  if (!blackBG->loadFromFile(gRenderer, "assets/global/img/blackBG.png")) 
    printf("Failed to load blackBG texture! SDL_Error: %s\n", SDL_GetError());
  else 
    blackBG->setDimension(SCREEN_WIDTH + 50, SCREEN_HEIGHT + 50);
  
  TTF_Font *font[TOTAL_TEXT];
  font[PAUSED] = TTF_OpenFont("assets/global/fonts/nexa-bold.otf", 120);
  font[CONTINUE] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 50);
  font[QUIT] = TTF_OpenFont("assets/global/fonts/nexa-light.otf", 50);

  const string txt[] = {"Continue", "Quit", "PAUSED"};
  for(int i = 0; i < TOTAL_TEXT; i++) {
    text[i] = new LTexture();
    if (!text[i]->loadFromRenderedText(gRenderer, font[i], txt[i], WHITE))
      printf("Failed to load %s text texture! SDL_Error: %s\n", txt[i].c_str(), SDL_GetError());

    if (i == PAUSED) break;
    blue_text[i] = new LTexture();
    if (!blue_text[i]->loadFromRenderedText(gRenderer, font[i], txt[i], ALLY_BLUE))
      printf("Failed to load %s selected text texture! SDL_Error: %s\n", txt[i].c_str(), SDL_GetError());
  }

  string prefix = "assets/global/sound/";
  const string sfxName[] = {"menu_select.wav", "setting_sfx.wav", "transition.wav"};
  for(int i = 0; i < TOTAL_SFX; i++) {
    sfx[i] = Mix_LoadWAV((prefix + sfxName[i]).c_str());
    if(sfx[i]== NULL) 
      printf( "Failed to load %s! SDL_mixer Error: %s\n", sfxName[i].c_str(), Mix_GetError());
  }

  refresh();
}

PauseScreen::~PauseScreen() {
  blackBG->free();
  for(int i = 0; i < TOTAL_TEXT; i++) {
    text[i]->free();
    if (i < PAUSED) blue_text[i]->free();
  }
  
}

int PauseScreen::handleKeyPress(const SDL_Event e) {
  if (SDL_KEYDOWN == e.type) {
    switch (e.key.keysym.sym) {
      case SDLK_LEFT:
      if (selected_section > CONTINUE) selected_section--;
      Mix_PlayChannel(-1, sfx[NAVIGATE], 0);
      break;
      case SDLK_RIGHT:
      if (selected_section < QUIT) selected_section++;
      Mix_PlayChannel(-1, sfx[NAVIGATE], 0);
      break;
      case SDLK_RETURN: {
        Mix_PlayChannel(-1, sfx[SELECT], 0);
        if (selected_section == CONTINUE) {
          int INGAME = 0;
          return INGAME;
        } else {
          int LEVEL_SELECT = 6;
          return LEVEL_SELECT;
        }
        break;
      }
    }
  }
  return -1;
}

void PauseScreen::render() {
  blackBG->render(gRenderer, 0, 0);

  for(int i = 0; i < TOTAL_TEXT; i++) {
    if (selected_section == i) 
      blue_text[i]->renderCenter(gRenderer, pos[i].x, pos[i].y, blue_text[i]->getWidth(), blue_text[i]->getHeight());
    else
      text[i]->renderCenter(gRenderer, pos[i].x, pos[i].y, text[i]->getWidth(), text[i]->getHeight());
  }
}

void PauseScreen::refresh() {
  selected_section = 0;
}
