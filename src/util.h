#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

using namespace std;

class LTexture {
public:
  // Initializes variables
  LTexture();
  // Deallocates memory
  ~LTexture();

  // Loads image at specified path
  bool loadFromFile(SDL_Renderer *renderer, std::string path);
  // Create image from font string
  bool loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font,
                            std::string textureText, SDL_Color textColor);
  // Deallocates texture
  void free();

  // Set alpha modulation
  void setAlpha(Uint8 alpha);

  // Renders texture at given point
  void render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = NULL,
              double angle = 0.0, SDL_Point *center = NULL,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Renders texture at given center
  void renderCenter(SDL_Renderer *renderer, int x_center, int y_center, int w,
                    int h, SDL_Rect *clip = NULL, double angle = 0.0,
                    SDL_Point *center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE);

  // Gets image dimensions
  int getWidth();
  int getHeight();
  int getX();
  int getY();

  void setDimension(int w, int h);
  void setPosition(int newX, int newY);

// protected:
  // The actual hardware texture
  SDL_Texture *mTexture;

  // Image dimensions
  int mWidth;
  int mHeight;
  int mPosX;
  int mPosY;
};

class LTimer {
public:
  // Initializes variables
  LTimer();

  // The various clock actions
  void start();
  void stop();
  void paused();
  void unpaused();

  // Gets the timer's time
  Uint32 getTicks();

  // Checks the status of the timer
  bool isStarted();
  bool isPaused();

private:
  // The clock time when the timer started
  Uint32 mStartTicks;

  // The ticks stored when the timer was paused
  Uint32 mPausedTicks;

  // The timer status
  bool mPaused;
  bool mStarted;
};

// Box collision detector
bool checkCollisionRotate(SDL_FRect a, SDL_FRect b, float angle);
float toRad(float degree);

// New coordinate of point when rotate angle degree 
// in unit circle FROM (0,0)
SDL_FPoint rotatePoint(float angle, SDL_FPoint p);
vector<SDL_FPoint> rotateRect(float angle, SDL_FRect r);
int randomNumber(int l, int r);
pair<int, int> shiftXY(float vel, float angle);

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);

struct Song {
  string name;
  string author;
  int duration;
  Mix_Music *music;
  Mix_Music *preview_music;
};

const int TOTAL_SONGS = 3;

extern Song SONG[TOTAL_SONGS];

int getNumLevel();
bool isFloat(string myString);
void loadMedia();

// The window we'll be rendering to and the window renderer
extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;

enum PageState {
  INGAME = 0,
  UNKNOWN,
  SETTING,
  EXIT,
  MENU,
  GAME_OVER,
  LEVEL_SELECT,
  TOTAL_STATES
};

enum FontType {
  NEXA_LIGHT = 0,
  NEXA_BOLD,
  NEXA_LIGHT_10,
  NEXA_BOLD_10,
  NEXA_BOLD_20,
  TOTAL_FONT
};
extern TTF_Font *gFont[TOTAL_FONT];

extern Mix_Music *gMusic;
// Level player are currently in
extern int gLevel;
extern int gMenuID;
// All textures need to be render
extern vector<LTexture> allTextures;
extern LTimer gTimer;
