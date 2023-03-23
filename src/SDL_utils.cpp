#include "SDL_utils.h"
#include "constants.h"

using namespace std;

bool init() {
  bool success = true;

  // Ininitialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else {
    gWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (NULL == gWindow) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      gRenderer = SDL_CreateRenderer(
          gWindow, -1, SDL_RENDERER_ACCELERATED); // remove vsync to cap fr
      if (NULL == gRenderer) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
      } else {
        // Ininitialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags)) {
          printf("SDL_image could not initialize! SDL_image Error: %s\n",
                 IMG_GetError());
          success = false;
        }
        // Ininitialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
          printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                 Mix_GetError());
          success = false;
        }
        // Ininitialize SDL_ttf
        if (TTF_Init() == -1) {
          printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                 TTF_GetError());
          success = false;
        }
      }
    }
  }
  return success;
}

bool loadMedia() { return false; }

void quitSDL() {
  SDL_DestroyWindow(gWindow);
  SDL_DestroyRenderer(gRenderer);
  gWindow = NULL;
  gRenderer = NULL;

  // Free the music
  Mix_FreeMusic(gMusic);
  gMusic = NULL;

  // Free the textures
  for (LTexture texture : allTextures) {
    texture.free();
  }
  allTextures.clear();

  // Free the font
  TTF_CloseFont(gFont);
  gFont = NULL;

  // Quit SDL subsystems
  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}
