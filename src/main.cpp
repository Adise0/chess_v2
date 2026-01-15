
#include "gameManager/GameManager.h"
#include "windowManager/WindowManager.h"
#include <iostream>
#include <windows.h>
namespace Chess {
using namespace Rendering;

int Run() {

  WindowManager::InitializeWindow();


  std::cout << "Running! \n";
  GameManager::Run();
  SDL_Quit();
  WindowManager::Dispose();
  return EXIT_SUCCESS;
}
} // namespace Chess


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  try {
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);
    }
    return Chess::Run();
  } catch (const std::runtime_error &e) {
    MessageBoxA(nullptr, e.what(), "Runtime error", MB_OK | MB_ICONERROR);
    return EXIT_FAILURE;
  }
}


