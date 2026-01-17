#pragma once
#include "../../types/Vector2.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <string>
namespace Chess::Rendering {
using namespace Types;

struct Styles {
public:
  Position position = Position::Static;
  SDL_Color backgroundColor = {0, 0, 0, 0};

  SDL_Texture *backgroundImage = nullptr;

  Display display = Display::Block;
  VerticalAlignment verticalAlignment = VerticalAlignment::Top;
  HorizontalAlignment horizontalAlignment = HorizontalAlignment::Left;
  FlexDirection flexDirection = FlexDirection::Row;
  int gap = 0;

  std::string topMargin = "";
  std::string rightMargin = "";
  std::string bottomMargin = "";
  std::string leftMargin = "";

  std::string topPadding = "";
  std::string rightPadding = "";
  std::string bottomPadding = "";
  std::string leftPadding = "";

  float zIndex = 0;

  std::string height = "";
  std::string width = "";

  std::string top = "";
  std::string left = "";

  float scale;
};
} // namespace Chess::Rendering
