#pragma once
#include "../../types/Vector2.h"
#include "types.h"
#include <SDL3/SDL.h>
namespace Chess::Rendering {
using namespace Types;

struct Styles {
public:
  Position position = Position::Relative;
  SDL_Color backgroundColor = {0, 0, 0, 0};

  SDL_Texture *backgroundImage = nullptr;

  Display display = Display::Flex;
  VerticalAlignment verticalAlignment = VerticalAlignment::Top;
  HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
  AlignDirection alignDirection = AlignDirection::Row;

  int topMargin = 0;
  int rightMargin = 0;
  int bottomMargin = 0;
  int leftMargin = 0;

  int topPadding = 0;
  int rightPadding = 0;
  int bottomPadding = 0;
  int leftPadding = 0;

  float zIndex = 0;

  int height = -1;
  int width = -1;

  int top = 0;
  int left = 0;

  float scale;
};
} // namespace Chess::Rendering
