#include "Element.h"
#include "../../windowManager/WindowManager.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>
namespace Chess::Rendering {

Element &Element::GetRoot() {
  static Element root("root");
  return root;
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

  if (!parent) return {0, 0, (float)styles.width, (float)styles.height};

  float x = parent->GetRect().x;
  float y = parent->GetRect().y;


  if (styles.position == Position::Relative) {
    x += GetLeftOffset(this);
    y += GetTopOffset(this);
  }
  if (styles.position == Position::Relative) {
    x += styles.left;
    y += styles.top;
  }

  return {x, y, (float)styles.width, (float)styles.height};
  // #endregion
}

SDL_Color &Element::GetDrawColor() {
  // #region GetDrawColor
  if (isActive && styles.activeColor) return *styles.activeColor;
  if (isHovered && styles.hoverColor) return *styles.hoverColor;
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
} // namespace Chess::Rendering
