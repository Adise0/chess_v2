#pragma once
#include "../ui/screens/mainMenu/MainMenu.h"
#include <SDL3/SDL.h>

namespace Chess {
using namespace Rendering::Screens;

class GameManager {
private:
  static bool isRunning;

public:
  static MainMenu mainMenu;
  static float deltaTime;

public:
  static void Run();

private:
  static void ProcessEvent(SDL_Event &event);
  static void Update();
  static void Render();
};
} // namespace Chess
