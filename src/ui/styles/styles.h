#pragma once
#include "types.h"
#include <SDL3/SDL.h>

namespace Chess::Rendering {
using namespace Types;

struct Styles {
public:
  SDL_Color backgroundColor;
  SDL_Color hoverColor;
  SDL_Color activeColor;

  SDL_Texture *backgroundImage;

  Display display;
  Alignment verticalAlignment;
  Alignment horizontalAlignment;

  int topMargin;
  int rightMargin;
  int bottomMargin;
  int leftMargin;

  int topPadding;
  int rightPadding;
  int bottomPadding;
  int leftPadding;
};
} // namespace Chess::Rendering
