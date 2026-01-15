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
  float width = GetWidth();
  float height = GetHeight();
  // std::cout << id << " has parent " << !!parent << std::endl;

  if (!parent) return {0, 0, width, height};

  float x = parent->GetRect().x;
  float y = parent->GetRect().y;


  if (styles.position == Position::Relative) {

    x += GetLeftOffset(this);
    y += GetTopOffset(this);

    x += parent->styles.leftPadding;
    y += parent->styles.topPadding;
    x += styles.leftMargin;
    y += styles.topMargin;

    if (parent->styles.horizontalAlignment == HorizontalAlignment::Center) {
      float parentWidth = parent->GetWidth();
      x += (parentWidth / 2) - (width / 2);
    }
    if (parent->styles.verticalAlignment == VerticalAlignment::Center) {
      float parentHeight = parent->GetHeight();
      y += (parentHeight / 2) - (height / 2);
    }
  }
  if (styles.position == Position::Absolute) {
    x += styles.left;
    y += styles.top;
  }
  // std::cout << id << x << ", " << y << ", " << width << ", " << height << std::endl;
  return {x, y, width, height};
  // #endregion
}

float Element::GetWidth() {
  // #region GetWidth
  if (!parent) return WindowManager::resolutionX;
  float width = styles.width;
  if (width == -1) width = parent->GetWidth();

  // width -= parent->styles.leftPadding + parent->styles.rightPadding;
  // width -= styles.leftMargin + styles.rightMargin;
  return width;
  // #endregion
}

float Element::GetHeight() {
  // #region GetHeight
  if (!parent) return WindowManager::resolutionY;
  float height = styles.height;
  if (height == -1) height = parent->GetHeight();

  // height -= parent->styles.topPadding + parent->styles.bottomPadding;
  // height -= styles.topMargin + styles.bottomMargin;

  return height;
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

int Element::GetLeftOffset(Element *child) {
  // #region GetLeftOffset
  if (!parent) throw std::runtime_error("Can't call GetTopOffset on root element");

  short index = parent->GetChildIndex(child);
  if (parent->styles.alignDirection == AlignDirection::Column) return 0;
  short offset = 0;
  for (short i = 0; i < index; i++) {
    Element *sibling = parent->children[i].get();
    if (sibling->styles.position == Position::Absolute) continue;
    offset += sibling->styles.leftMargin;
    offset += sibling->styles.width;
    offset += sibling->styles.rightMargin;
  }

  return offset;
  // #endregion
}

int Element::GetTopOffset(Element *child) {
  // #region GetTopOffset
  if (!parent) throw std::runtime_error("Can't call GetTopOffset on root element");

  short index = parent->GetChildIndex(child);
  if (parent->styles.alignDirection == AlignDirection::Row) return 0;
  short offset = 0;
  for (short i = 0; i < index; i++) {
    Element *sibling = parent->children[i].get();
    if (sibling->styles.position == Position::Absolute) continue;
    offset += sibling->styles.topMargin;
    offset += sibling->styles.height;
    offset += sibling->styles.bottomMargin;
  }

  return offset;
  // #endregion
}

void Element::HandleEvent(SDL_Event &sdlEvent, Event &event) {
  // #region HandleEvent
  if (event.stopPropagation) return;
  if (sdlEvent.button.x >= GetRect().x && sdlEvent.button.x <= GetRect().x + GetWidth() &&
      sdlEvent.button.y >= GetRect().y && sdlEvent.button.y <= GetRect().y + GetHeight()) {
    if (!isHovered) {
      isHovered = true;
      OnHoverHandler(event);
    }
  } else if (isHovered) isHovered = false;

  for (short i = 0; i < children.size(); i++) {
    children[i]->HandleEvent(sdlEvent, event);
  }

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
