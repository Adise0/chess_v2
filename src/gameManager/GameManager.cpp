#include "GameManager.h"
#include "../windowManager/WindowManager.h"
#include <chrono>

namespace Chess {

using namespace Rendering;


bool GameManager::isRunning = false;
float GameManager::deltaTime = 0;


void GameManager::Run() {
  // #region Run
  isRunning = true;
  SDL_Event event;
  auto lastFrame = std::chrono::high_resolution_clock::now();


  while (isRunning) {
    // ProcessInput(event);
    auto thisFrame = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(thisFrame - lastFrame).count();
    lastFrame = thisFrame;

    ProcessEvent(event);
    Update();
    Render();
  }
  // #endregion
}


void GameManager::ProcessEvent(SDL_Event &event) {
  // #region ProcessEvent
  while (SDL_PollEvent(&event)) {

    switch (event.type) {
    case SDL_EVENT_QUIT:
      isRunning = false;
      break;

    default:
      break;
    }
  }
  // #endregion
}

void GameManager::Update() {
  // #region Update
  // Input
  // Process
  // Render
  // #endregion
}

void GameManager::Render() {
  // #region Render
  SDL_RenderClear(WindowManager::renderer);

  SDL_RenderPresent(WindowManager::renderer);
  // #endregion
}
} // namespace Chess
