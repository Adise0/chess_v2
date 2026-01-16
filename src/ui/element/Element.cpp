#include "Element.h"
#include "../../windowManager/WindowManager.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>
namespace Chess::Rendering {

Element &Element::GetRoot() {
  // #region GetRoot
  static Element root("root");
  return root;
  // #endregion
}

Element::Element(std::string id) : id(id) {}

Element::~Element() {}

Element *Element::CreateChild(std::string id) {
  // #region CreateChild
  auto child = std::make_unique<Element>(std::move(id));
  Element *raw = child.get();
  AppendChild(std::move(child));
  return raw;
  // #endregion
}

void Element::AppendChild(std::unique_ptr<Element> child) {
  // #region AppendChild
  if (!child) return;

  if (child->parent && child->parent != this) {
    auto detachedChild = child->parent->RemoveChild(child.get());
    if (!detachedChild) return;
    child = std::move(detachedChild);
  }
  child->parent = this;
  children.push_back(std::move(child));
  // #endregion
}

std::unique_ptr<Element> Element::RemoveChild(Element *child) {
  // #region Removechild
  if (!child) return nullptr;

  auto &siblings = child->parent->children;
  auto it =
      std::find_if(siblings.begin(), siblings.end(),
                   [&](const std::unique_ptr<Element> &sibling) { return sibling.get() == child; });

  if (it == siblings.end()) return nullptr;

  auto out = std::move(*it);
  children.erase(it);
  out->parent = nullptr;
  return out;
  // #endregion
}

void Element::Render() {
  // #region Render
  if (styles.display == Display::None) return;

  SDL_Color &color = GetDrawColor();
  SDL_SetRenderDrawColor(WindowManager::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(WindowManager::renderer, &GetRect());

  SDL_Texture *texture = GetTexture();
  if (texture) SDL_RenderTexture(WindowManager::renderer, texture, NULL, &GetRect());

  std::vector<Element *> elements = GetRenderElements();
  for (Element *element : elements) {
    element->Render();
  }


  // #endregion
}

SDL_FRect Element::GetRect() {
  // #region GetRect
  if (!parent) return {0, 0, (float)WindowManager::resolutionX, (float)WindowManager::resolutionY};

  return parent->GetChildRect(this);
  // #endregion
}


SDL_Color &Element::GetDrawColor() {
  // #region GetDrawColor
  return styles.backgroundColor;
  // #endregion
}

SDL_Texture *Element::GetTexture() { return styles.backgroundImage; }


std::vector<Element *> Element::GetRenderElements() {
  // #region GetRenderElements
  std::vector<Element *> elementsToRender;
  for (short i = 0; i < children.size(); i++) {
    if (children[i]->styles.display == Display::None) continue;
    elementsToRender.push_back(children[i].get());
  }

  std::stable_sort(
      elementsToRender.begin(), elementsToRender.end(),
      [](const Element *a, const Element *b) { return a->styles.zIndex > b->styles.zIndex; });

  return elementsToRender;
  // #endregion
}

short Element::GetChildIndex(Element *child) {
  // #region GetChildIndex
  if (!child) throw std::runtime_error("Unknown child");

  auto it =
      std::find_if(children.begin(), children.end(),
                   [&](const std::unique_ptr<Element> &sibling) { return sibling.get() == child; });

  if (it == children.end()) throw std::runtime_error("Unknown child");
  return (short)(std::distance(children.begin(), it));
  // #endregion
}

short Element::GetSiblingIndex() {
  // #region GetSiblingIndex
  if (!parent) throw std::runtime_error("Cannot call GetSiblingIndex on root");
  return parent->GetChildIndex(this);
  // #endregion
}

void Element::SetSiblingIndex(short index) {
  // #region SetSiblingIndex
  if (!parent) return;
  parent->SetChildIndex(this, index);
  // #endregion
}

void Element::SetChildIndex(Element *child, short newIndex) {
  // #region SetChildIndex
  if (!child) return;
  if (newIndex < 0 || newIndex >= children.size()) {
    throw std::runtime_error("Cannot set child " + child->id + " to index " +
                             std::to_string(newIndex) + ". Index out of range.");
  }

  short oldIndex = GetChildIndex(child);

  auto replacedChild = std::move(children[newIndex]);
  auto childToMove = std::move(children[oldIndex]);

  children[newIndex] = nullptr;
  children[oldIndex] = nullptr;

  children[newIndex] = std::move(childToMove);
  children[oldIndex] = std::move(replacedChild);
  // #endregion
}

bool Element::IsWithinRect(Vector2 position) {
  // #region IsWithinRect
  SDL_FRect rect = GetRect();
  return position.x >= rect.x && position.x <= rect.x + rect.w && position.y >= rect.y &&
         position.y <= rect.y + rect.h;
  // #endregion
}

Element *Element::GetRelativeParent() {
  if (!parent || styles.position == Position::Relative) return this;
  return parent->GetRelativeParent();
}

SDL_FRect Element::GetChildRect(Element *child) {
  // #region GetChildRect
  SDL_FRect myRect = GetRect();
  SDL_FRect childRect = {myRect.x, myRect.y, 0, 0};
  short childIndex = GetChildIndex(child);

  if (child->styles.position == Position::Absolute) {
    SDL_FRect relativeParentRect = GetRelativeParent()->GetRect();

    childRect.x = relativeParentRect.x + styles.left + styles.leftMargin;
    childRect.y = relativeParentRect.y + styles.top + styles.topMargin;

    int width = child->styles.width;
    width == -1 ? width = relativeParentRect.w : width;

    int height = child->styles.height;
    height == -1 ? height = relativeParentRect.h : height;

    return childRect;
  }

  for (size_t i = 0; i < children.size(); i++) {
  }


  return childRect;
  // #endregion
}

void Element::HandleEvent(SDL_Event &sdlEvent, Event &event) {
  // #region HandleEvent


  if (event.stopPropagation) return;
  for (short i = 0; i < children.size(); i++) {
    children[i]->HandleEvent(sdlEvent, event);
  }
  Vector2 mousePos(sdlEvent.motion.x, sdlEvent.motion.y);

  if (IsWithinRect(mousePos) && !isHovered) {
    if (!isHovered) {
      isHovered = true;
      OnHoverHandler(event);
    }
  } else if (isHovered) isHovered = false;


  // #endregion
}

void Element::OnClick(std::function<void(Event &event)> listener) {
  // #region OnClick
  onClickListeners.push_back(listener);
  // #endregion
}

void Element::OnHover(std::function<void(Event &event)> listener) {
  // #region OnClick
  onHoverListeners.push_back(listener);
  // #endregion
}

void Element::OnClickHandler(Event &event) {
  // #region OnClickHandler
  for (std::function<void(Event &event)> listener : onClickListeners) {
    listener(event);
  }
  // #endregion
}

void Element::OnHoverHandler(Event &event) {
  // #region OnHoverHandler
  for (std::function<void(Event &event)> listener : onHoverListeners) {
    listener(event);
  }
  // #endregion
}
} // namespace Chess::Rendering
