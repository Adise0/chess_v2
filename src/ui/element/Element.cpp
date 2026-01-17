#include "Element.h"
#include "../../windowManager/WindowManager.h"
#include "../styles/styleParser/StyleParser.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace Chess::Rendering {

bool Element::isInitialized = false;
Styles Element::rootStyles{
    .backgroundColor = {255, 0, 255, 255},
    .width = "100vw",
    .height = "100vh",
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
  if (id == "MainMenu") {
    std::cout << "From render: " << rect.x << " " << rect.y << std::endl;
  }


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
  if (!parent) return {0, 0, (float)WindowManager::resolutionX, (float)WindowManager::resolutionY};

  return parent->GetChildRect(this);
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
  if (!parent || styles.position == Position::Relative) return this;
  return parent->GetRelativeParent();
  // #endregion
}

SDL_FRect Element::GetChildRect(Element *child) {
  // #region GetChildRect
  SDL_FRect myRect = GetRect();
  SDL_FRect childRect = {myRect.x, myRect.y, 0, 0};


  int topPadding = StyleParser::GetStyleValue(this, "topPadding");
  int rightPadding = StyleParser::GetStyleValue(this, "rightPadding");
  int bottomPadding = StyleParser::GetStyleValue(this, "bottomPadding");
  int leftPadding = StyleParser::GetStyleValue(this, "leftPadding");

  int width = StyleParser::GetStyleValue(child, "width");
  int height = StyleParser::GetStyleValue(child, "height");

  if (child->id == "MainMenu") {
    std::cout << "Width: " << width << " Height: " << height << std::endl;
  }

  childRect.w = width;
  childRect.h = height;

  if (child->styles.position == Position::Absolute) {
    SDL_FRect relativeParentRect = GetRelativeParent()->GetRect();
    int left = StyleParser::GetStyleValue(child, "left");
    int top = StyleParser::GetStyleValue(child, "top");

    childRect.x = relativeParentRect.x + left;
    childRect.y = relativeParentRect.y + top;

    return childRect;
  }


  short childIndex = GetChildIndex(child);
  int currentX = 0;
  int currentY = 0;
  int rowY;
  int colX;
  for (short i = 0; i < childIndex; i++) {
    int siblingTopMargin = StyleParser::GetStyleValue(children[i].get(), "topMargin");
    int siblingRightMargin = StyleParser::GetStyleValue(children[i].get(), "rightMargin");
    int siblingBottomMargin = StyleParser::GetStyleValue(children[i].get(), "bottomMargin");
    int siblingLeftMargin = StyleParser::GetStyleValue(children[i].get(), "leftMargin");
    int siblingWidth = StyleParser::GetStyleValue(children[i].get(), "width");
    int siblingHeight = StyleParser::GetStyleValue(children[i].get(), "height");

    if (styles.flexDirection == FlexDirection::Row) {
      rowY = std::max(siblingHeight, rowY);
      currentX += siblingLeftMargin + siblingWidth + siblingRightMargin + styles.gap;
      if (currentX + width > (myRect.w - (leftPadding + rightPadding))) {
        currentX = 0;
        currentY += rowY;
        rowY = 0;
      }
    } else {
      colX = std::max(siblingWidth, colX);
      currentY += siblingTopMargin + siblingHeight + siblingBottomMargin + styles.gap;
      if (currentY + height > (myRect.h - (topPadding + bottomPadding))) {
        currentY = 0;
        currentX += colX;
        colX = 0;
      }
    }
  }

  childRect.x = myRect.x + currentX;
  childRect.y = myRect.y + currentY;

  return childRect;
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
