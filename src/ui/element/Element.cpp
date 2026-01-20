#include "Element.h"
#include "../../windowManager/WindowManager.h"
#include "../styles/styleParser/StyleParser.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace Chess::Rendering {

bool Element::isInitialized = false;
Styles Element::rootStyles{
    .width = "100vw",
    .height = "100vh",
    .backgroundColor = {255, 0, 255, 255},
};


Element &Element::GetRoot() {
  // #region GetRoot
  static Element root("root");
  if (!isInitialized) {
    root.styles = rootStyles;
    isInitialized = true;
  }

  return root;
  // #endregion
}


bool Element::IsElementInFlow(Element *element) {
  // #region IsElementInFlow
  return (element->styles.position == Position::Relative ||
          element->styles.position == Position::Static) &&
         element->styles.display != Display::None;
  // #endregion
}

SDL_FRect Element::GetDisplayRect(Element *element) {
  // #region GetDisplayRect
  SDL_FRect outerRect = GetOuterRect(element);

  int topMargin = StyleParser::GetStyleValue(element, "topMargin");
  int rightMargin = StyleParser::GetStyleValue(element, "rightMargin");
  int bottomMargin = StyleParser::GetStyleValue(element, "bottomMargin");
  int leftMargin = StyleParser::GetStyleValue(element, "leftMargin");

  float x = outerRect.x + leftMargin;
  float y = outerRect.y + topMargin;
  float w = outerRect.w - rightMargin;
  float h = outerRect.h - bottomMargin;

  // TODO: Add truncating from overflow, scroll and shi

  return {x, y, w, h};
  // #endregion
}
SDL_FRect Element::GetInnerRect(Element *element) {
  // #region GetInnerRect
  SDL_FRect outerRect = GetOuterRect(element);

  int topPadding = StyleParser::GetStyleValue(element, "topPadding");
  int rightPadding = StyleParser::GetStyleValue(element, "rightPadding");
  int bottomPadding = StyleParser::GetStyleValue(element, "bottomPadding");
  int leftPadding = StyleParser::GetStyleValue(element, "leftPadding");


  int topMargin = StyleParser::GetStyleValue(element, "topMargin");
  int rightMargin = StyleParser::GetStyleValue(element, "rightMargin");
  int bottomMargin = StyleParser::GetStyleValue(element, "bottomMargin");
  int leftMargin = StyleParser::GetStyleValue(element, "leftMargin");

  float x = outerRect.x + leftMargin + leftPadding;
  float y = outerRect.y + topMargin + topPadding;
  float w = outerRect.w - rightPadding - rightPadding;
  float h = outerRect.h - bottomPadding - bottomPadding;

  return {x, y, w, y};
  // #endregion
}

SDL_FRect Element::GetOuterRect(Element *element) {
  // #region GetOuterRect
  if (!element->parent)
    return {0, 0, (float)WindowManager::resolutionX, (float)WindowManager::resolutionY};

  SDL_FRect parentInnerRect = GetInnerRect(element->parent);

  if (element->parent->styles.display == Display::Block) {
    float x = parentInnerRect.x;
    float y = parentInnerRect.y;

    short siblingIndex = element->GetSiblingIndex();
    for (short i = 0; i < siblingIndex; i++) {
      Element *sibling = element->parent->children[i].get();
      if (!IsElementInFlow(sibling)) continue;
      y += GetOuterRect(sibling).h;
    }

    int topMargin = StyleParser::GetStyleValue(element, "topMargin");
    int rightMargin = StyleParser::GetStyleValue(element, "rightMargin");
    int bottomMargin = StyleParser::GetStyleValue(element, "bottomMargin");
    int leftMargin = StyleParser::GetStyleValue(element, "leftMargin");

    float width = StyleParser::GetStyleValue(element, "width");
    float height = StyleParser::GetStyleValue(element, "height");

    width += leftMargin + rightMargin;
    height += topMargin + bottomMargin;

    SDL_FRect rect = {x, y, width, height};
    if (element->parent->styles.overflow == Overflow::Show) return rect;
    // TODO: scroll

    if (element->parent->styles.overflow == Overflow::Hidden) {
      if (!element->parent->IsWithinRect(Vector2(rect.x, rect.y))) {
        rect.w = 0;
        rect.h = 0;
      }
    }
    return rect;
  }
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

  SDL_FRect rect = GetRect();

  SDL_Color &color = styles.backgroundColor;
  SDL_SetRenderDrawColor(WindowManager::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(WindowManager::renderer, &rect);

  SDL_Texture *texture = styles.backgroundImage;
  if (texture) SDL_RenderTexture(WindowManager::renderer, texture, NULL, &rect);

  std::vector<Element *> elements = GetRenderElements();
  for (Element *element : elements) {
    element->Render();
  }


  // #endregion
}

SDL_FRect Element::GetRect() {
  // #region GetRect
  return GetDisplayRect(this);
  // #endregion
}

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
  // #region GetRelativeParent
  if (!parent || styles.position != Position::Static) return this;
  return parent->GetRelativeParent();
  // #endregion
}

void Element::HandleEvent(Event &event) {
  // #region HandleEvent
  if (event.stopPropagation) return;
  for (short i = 0; i < children.size(); i++) {
    children[i]->HandleEvent(event);
  }
  if (event.stopPropagation) return;

  eventHandler.HandleEvent(event);
  // #endregion
}
} // namespace Chess::Rendering
