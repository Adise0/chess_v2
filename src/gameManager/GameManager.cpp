#include "GameManager.h"
#include "../ui/element/Element.h"
#include "../windowManager/WindowManager.h"
#include <chrono>
#include <iostream>
#include <string>

namespace Chess {

using namespace Rendering;

bool GameManager::isRunning = false;
float GameManager::deltaTime = 0;
MainMenu GameManager::mainMenu = MainMenu();

void GameManager::Run() {
  // #region Run
  isRunning = true;
  SDL_Event event;
  auto lastFrame = std::chrono::high_resolution_clock::now();


  // mainMenu.Show();


  while (isRunning) {
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
  SDL_SetRenderDrawColor(WindowManager::renderer, 0, 0, 0, 255);
  SDL_RenderClear(WindowManager::renderer);


  Element::GetRoot().Render();

  SDL_RenderPresent(WindowManager::renderer);
  // #endregion
}
} // namespace Chess
