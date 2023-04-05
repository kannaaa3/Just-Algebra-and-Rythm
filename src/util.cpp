#include "util.h"
#include "constants.h"
#include <iostream>
using namespace std;

LTexture::LTexture() {
  // Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  mPosX = 0;
  mPosY = 0;
}

LTexture::~LTexture() {
  // Deallocate
  free();
}

void LTexture::setDimension(int w, int h) {
  mWidth = w;
  mHeight = h;
}

bool LTexture::loadFromFile(SDL_Renderer *renderer, std::string path) {
  // Get rid of preexisting texture
  free();
  // The final texture
  SDL_Texture *newTexture = NULL;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (NULL == loadedSurface) {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
           IMG_GetError());
  } else {
    // Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    // Create texture from surface pixel
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (NULL == newTexture) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    } else {
      // Get image dimensions
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  // return success
  mTexture = newTexture;
  return NULL != mTexture;
}

bool LTexture::loadFromRenderedText(SDL_Renderer *renderer, TTF_Font *font,
                                    std::string textureText,
                                    SDL_Color textColor) {
  // Get rid of preexisting texture
  free();

  // Render text surface
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(font, textureText.c_str(), textColor);
  if (NULL == textSurface) {
    printf("Unable to render text surface! SDL_ttf Error: %s\n",
           TTF_GetError());
  } else {
    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (NULL == mTexture) {
      printf("Unable to create texture from rendered text! SDL Error: %s\n",
             SDL_GetError());
    } else {
      // Get image dimensions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }

    // Get rid of old surface
    SDL_FreeSurface(textSurface);
  }

  // Return success
  return mTexture != NULL;
}

void LTexture::free() {
  // Free texture if it exists
  if (NULL != mTexture) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::setAlpha(Uint8 alpha) {
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip,
                      double angle, SDL_Point *center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  // Set clip rendering dimensions
  if (NULL != clip) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  // Render to screen
  SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, -angle, center, flip);
}

void LTexture::renderCenter(SDL_Renderer *renderer, int x_center, int y_center,
                            int w, int h, SDL_Rect *clip, double angle,
                            SDL_Point *center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen, w, h for sprites
  SDL_Rect renderQuad = {x_center - w / 2, y_center - h / 2, mWidth, mHeight};

  // Set clip rendering dimensions
  if (NULL != clip) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  // Render to screen
  SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, -angle, center, flip);
}

void LTexture::setPosition(int newX, int newY) {
  mPosX = newX;
  mPosY = newY;
}

int LTexture::getWidth() { return mWidth; }
int LTexture::getHeight() { return mHeight; }
int LTexture::getX() { return mPosX; }
int LTexture::getY() { return mPosY; }

LTimer::LTimer() {
  // Initialize the variables
  mStartTicks = 0;
  mPausedTicks = 0;

  mPaused = false;
  mStarted = false;
}

void LTimer::start() {
  // Start the timer
  mStarted = true;

  // Unpaused the timer
  mPaused = false;

  // Get the current clock time
  mStartTicks = SDL_GetTicks();
  mPausedTicks = 0;
}

void LTimer::stop() {
  // Stop the timer
  mStarted = false;

  // Unpaused the timer
  mPaused = false;

  // Clear tick variables
  mStartTicks = 0;
  mPausedTicks = 0;
}

void LTimer::paused() {
  // if the timer is running and isn't already paused
  if (mStarted && !mPaused) {
    // Pause the timer
    mPaused = true;

    // Calculate the paused ticks
    mPausedTicks = SDL_GetTicks() - mStartTicks;
    mStartTicks = 0;
  }
}

void LTimer::unpaused() {
  if (mStarted && mPaused) {
    // Unpause the timer
    mPaused = false;

    // Reset the starting ticks
    mStartTicks = SDL_GetTicks() - mPausedTicks;

    // Reset the paused ticks
    mPausedTicks = 0;
  }
}

Uint32 LTimer::getTicks() {
  Uint32 time = 0;

  if (mStarted) {
    // Return the number of ticks when the timer was paused
    if (mPaused) {
      time = mPausedTicks;
    } else {
      // Return the current time minus the start time
      time = SDL_GetTicks() - mStartTicks;
    }
  }

  return time;
}

bool LTimer::isStarted() {
  // Timer is running and paused or unpaused
  return mStarted;
}

bool LTimer::isPaused() {
  // Timer is running and paused
  return mPaused && mStarted;
}

// Check whether orientation of 3 ordered points is counter_clockwise or not
bool counter_clockwise (SDL_FPoint p1, SDL_FPoint p2, SDL_FPoint p3) {
  return ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y) > 0);
}

// Rotate axis respect to angle of a, and check if any corner of b inside a
bool checkCollisionRotate(SDL_FRect a, SDL_FRect b, float angle) {
  // Corner of rectangle in counter clockwise order
  vector<SDL_FPoint> corners = rotateRect(angle, a);
  // cout << "Four point:\n";
  // for(int i = 0; i < 4; i++) 
  //   cout << corners[i].x << " " << corners[i].y << endl;

  float sX[] = {-1, -1, 1, 1};
  float sY[] = {1, -1, 1, -1};
  for(int i = 0; i < 4; i++) {
    bool inside = true;
    SDL_FPoint p = {b.x + sX[i] * b.w / 2, b.y + sY[i] * b.h / 2};

    for(int j = 0; j < 4; j++) {
      inside &= counter_clockwise(corners[(j+1) % 4], corners[j], p);
    }
    if (inside) return true;
  }
  return false;
}
float toRad(float degree) {
  return degree * M_PI / 180.0;
}

SDL_FPoint rotatePoint(float angle, SDL_FPoint p) {
  float cosA = cos(-angle * M_PI / 180);
  float sinA = sin(-angle * M_PI / 180);
  SDL_FPoint ans = {p.x * cosA - p.y * sinA, p.x * sinA + p.y * cosA};
  return ans;
}

vector<SDL_FPoint> rotateRect(float angle, SDL_FRect r) {
  // Rect r: known center and dimension
  // Four corner in counterclockwise
  vector<SDL_FPoint> corners = {
    {r.x - r.w / 2, r.y - r.h / 2},
    {r.x - r.w / 2, r.y + r.h / 2},
    {r.x + r.w / 2, r.y + r.h / 2},
    {r.x + r.w / 2, r.y - r.h / 2},
  };

  for (auto &p : corners) {
    // Move the center to (0,0)
    p.x -= r.x; p.y -= r.y;
    p = rotatePoint(angle, p);
    // Move the center back
    p.x += r.x; p.y += r.y;
  }

  return corners;
}

int randomNumber(int l, int r) { return l + rand() % (r - l + 1); }
pair<int, int> shiftXY(float vel, float dir) {
  return {vel * cos(dir * M_PI / 180), -vel * sin(dir * M_PI / 180)};
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius) {
  SDL_SetRenderDrawColor(gRenderer, WHITE.r, WHITE.g, WHITE.b, 100);
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font* gFont[TOTAL_FONT] = {NULL, NULL, NULL, NULL, NULL};
Mix_Music *gMusic;
// Level player are currently in
int gLevel = 0;
int gMenuID = 0;
LTimer gTimer;
