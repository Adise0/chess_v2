#pragma once
#include <SDL3/SDL.h>

namespace Chess {

class GameManager {
private:
  static bool isRunning;

public:
  static float deltaTime;

public:
  static void Run();

private:
  static void ProcessEvent(SDL_Event &event);
  static void Update();
  static void Render();
};
} // namespace Chess
