#pragma once
#include <SDL3/SDL.h>
#include <map>
#include <string>
namespace Chess::Rendering {
class WindowManager {
private:
  /// @brief The SDL window instance
  static SDL_Window *window;

public:
  /// @brief The SDL renderer instance
  static SDL_Renderer *renderer;

  /// @brief The width of the window
  const static short resolutionX;
  /// @brief The height of the window
  const static short resolutionY;

  /// @brief The loaded sprite PATHS
  static std::map<std::string, SDL_Texture *> loadedSprites;


public:
  /// @brief Creates and initializes the window manager
  static void InitializeWindow();

  /// @brief Loads a sprite
  /// @param spritePath The sprite path
  /// @return The loaded texture
  static SDL_Texture *LoadSprite(std::string spritePath);

  /// @brief Disposes and cleans the window manager
  static void Dispose();
};
} // namespace Chess::Rendering
