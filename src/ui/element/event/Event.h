#pragma once
#include "../../../types/Vector2.h"
#include <SDL3/SDL.h>
namespace Chess::Rendering::Events {


enum EventType { None, OnClick, OnMouseMove };

struct Event {
public:
  EventType eventType;
  Vector2 mousePos;
  SDL_Event &sdlEvent;
  bool stopPropagation = false;

  Event(SDL_Event &sdlEvent) : sdlEvent(sdlEvent) {};
};
} // namespace Chess::Rendering::Events
