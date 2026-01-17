#pragma once
#include "../../../types/Vector2.h"
#include <SDL3/SDL.h>
namespace Chess::Rendering::Events {


enum EventType { OnClick, OnMouseEnter, OnMouseLeave };

struct Event {
public:
  EventType eventType;
  Vector2 mousePos;
  SDL_Event &sdlEvent;
  bool stopPropagation = false;
};
} // namespace Chess::Rendering::Events
