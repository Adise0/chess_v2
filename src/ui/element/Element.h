#pragma once
#include "../styles/styles.h"
#include "event/Event.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Chess::Rendering {
using namespace Events;
class Element {
public:
  static Element &GetRoot();

public:
  Styles styles;
  std::string id;

  std::vector<std::unique_ptr<Element>> children;
  Element *parent = nullptr;

  bool isHovered = false;
  bool isActive = false;

private:
  std::vector<std::function<void(Event &event)>> onClickListeners;
  std::vector<std::function<void(Event &event)>> onHoverListeners;

public:
  Element(std::string id);
  ~Element();


  void Render();
  void AppendChild(std::unique_ptr<Element> child);
  Element *CreateChild(std::string id);

  SDL_FRect GetRect();

  short GetSiblingIndex();
  void SetSiblingIndex(short newIndex);

  void OnClick(std::function<void(Event &event)> listener);
  void OnHover(std::function<void(Event &event)> listener);

  void HandleEvent(SDL_Event &sdlEvent, Event &event);


private:
  short GetChildIndex(Element *child);
  void SetChildIndex(Element *child, short newIndex);

  int GetLeftOffset(Element *child);
  int GetTopOffset(Element *child);
  std::unique_ptr<Element> RemoveChild(Element *child);

  SDL_Color &GetDrawColor();
  SDL_Texture *GetTexture();
  std::vector<Element *> GetRenderElements();

  float GetHeight();
  float GetWidth();

  void OnHoverHandler(Event &event);
  void OnClickHandler(Event &event);
};


} // namespace Chess::Rendering
