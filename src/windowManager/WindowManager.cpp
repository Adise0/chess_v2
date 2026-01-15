#include "WindowManager.h"
#include <SDL3_image/SDL_image.h>
#include <iostream>


namespace Chess::Rendering {


std::map<std::string, SDL_Texture *> WindowManager::loadedSprites;
SDL_Window *WindowManager::window = nullptr;
SDL_Renderer *WindowManager::renderer = nullptr;

const short WindowManager::resolutionX = 1280;
const short WindowManager::resolutionY = 800;



void WindowManager::InitializeWindow() {
  // #region InitializeWindow
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error("Failed to initialize SDL video subsystem");

  window = SDL_CreateWindow("Chess", resolutionX, resolutionY, 0);
  if (!window) throw std::runtime_error("Failed to create SDL window");

  renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) throw std::runtime_error("Failed to create SDL renderer");
  // #endregion
}

SDL_Texture *WindowManager::LoadSprite(std::string spritePath) {
  // #region LoadSprite

  if (loadedSprites.find(spritePath) != loadedSprites.end()) {
    return loadedSprites[spritePath];
  }


  SDL_Texture *texture = IMG_LoadTexture(renderer, spritePath.c_str());
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

  loadedSprites[spritePath] = texture;
  return texture;
  // #endregion
}

void WindowManager::Dispose() {
  // #region Dispose
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }

  SDL_Quit();
  // #endregion
}
} // namespace Chess::Rendering
