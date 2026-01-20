#pragma once
#include "../styles/styles.h"
#include "event/Event.h"
#include "eventHandler/EventHandler.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Chess::Rendering {
using namespace Events;
class Element {
  // #region Static
private:
  static bool isInitialized;
  static Styles rootStyles;

public:
  static Element &GetRoot();
  static SDL_FRect GetDisplayRect(Element *element);
  static SDL_FRect GetInnerRect(Element *element);
  static SDL_FRect GetOuterRect(Element *element);

  static bool IsElementInFlow(Element *element);
  // #endregion

  // #region Data
public:
  Styles styles;
  std::string id;
  EventHandler eventHandler;

  std::vector<std::unique_ptr<Element>> children;
  Element *parent = nullptr;

  bool isHovered = false;
  bool isActive = false;

  float scrollPosition = 0;
  // #endregion

  // #region Constructors
public:
  Element(std::string id);
  ~Element();
  // #endregion

  // #region Methods
public:
  void Render();
  void AppendChild(std::unique_ptr<Element> child);
  Element *CreateChild(std::string id);

  SDL_FRect GetRect();

  short GetSiblingIndex();
  void SetSiblingIndex(short newIndex);

  void HandleEvent(Event &event);

private:
  short GetChildIndex(Element *child);
  void SetChildIndex(Element *child, short newIndex);

  int GetLeftOffset(Element *child);
  int GetTopOffset(Element *child);
  std::unique_ptr<Element> RemoveChild(Element *child);
  std::vector<Element *> GetRenderElements();
  bool IsWithinRect(Vector2 position);
  Element *GetRelativeParent();


  // #endregion

  // #region Aliases
public:
  void On(EventType eventType, std::function<void(Event &event)> listener);
  // #endregion
};


} // namespace Chess::Rendering
