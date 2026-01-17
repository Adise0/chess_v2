#pragma once
#include "../../types/Vector2.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <stdexcept>
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
  bool flex;

  std::string topMargin = "default";
  std::string rightMargin = "default";
  std::string bottomMargin = "default";
  std::string leftMargin = "default";

  std::string topPadding = "default";
  std::string rightPadding = "default";
  std::string bottomPadding = "default";
  std::string leftPadding = "default";

  float zIndex = 0;

  std::string width = "default";
  std::string height = "default";

  std::string top = "default";
  std::string left = "default";

  float scale;

  std::string Get(std::string name) {
    // #region Get
    if (name == "topMargin") return topMargin;
    if (name == "rightMargin") return rightMargin;
    if (name == "bottomMargin") return bottomMargin;
    if (name == "leftMargin") return leftMargin;
    if (name == "topPadding") return topPadding;
    if (name == "rightPadding") return rightPadding;
    if (name == "bottomPadding") return bottomPadding;
    if (name == "leftPadding") return leftPadding;
    if (name == "height") return height;
    if (name == "width") return width;
    if (name == "top") return top;
    if (name == "left") return left;
    throw std::runtime_error("Unknnown style parameter: " + name);
    // #endregion
  }
};
} // namespace Chess::Rendering
