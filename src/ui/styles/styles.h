#pragma once
#include "../../types/Vector2.h"
#include "types.h"
#include <SDL3/SDL.h>
namespace Chess::Rendering {
using namespace Types;

struct Styles {
public:
  Position position = Position::Relative;

  SDL_Color backgroundColor{0, 0, 0, 255};
  SDL_Color hoverColor{-1, -1, -1, -1};
  SDL_Color activeColor{-1, -1, -1, -1};

  SDL_Texture *backgroundImage = nullptr;

  Display display = Display::Flex;
  VecticalAlignment verticalAlignment = VecticalAlignment::Top;
  HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;

  int topMargin = 0;
  int rightMargin = 0;
  int bottomMargin = 0;
  int leftMargin = 0;

  int topPadding = 0;
  int rightPadding = 0;
  int bottomPadding = 0;
  int leftPadding = 0;
};
} // namespace Chess::Rendering
