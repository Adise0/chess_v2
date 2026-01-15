#include "Element.h"
#include "../../windowManager/WindowManager.h"
#include <algorithm>
#include <iostream>
namespace Chess::Rendering {

Element &Element::GetRoot() {
  static Element root("root");
  return root;
}

Element::Element(std::string id) : id(id) {}

Element::~Element() {}

Element *Element::CreateChild(std::string id) {
  auto child = std::make_unique<Element>(std::move(id));
  Element *raw = child.get();
  AppendChild(std::move(child));
  return raw;
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
  float x = 0;
  float y = 0;

  if (parent) {
    x += parent->GetRect().x;
    y += parent->GetRect().y;
  }
  return {x, y, (float)styles.width, (float)styles.height};
}

SDL_Color &Element::GetDrawColor() {
  if (isActive && styles.activeColor) return *styles.activeColor;
  if (isHovered && styles.hoverColor) return *styles.hoverColor;
  return styles.backgroundColor;
}

SDL_Texture *Element::GetTexture() { return styles.backgroundImage; }


std::vector<Element *> Element::GetRenderElements() {

  std::vector<Element *> elementsToRender;
  for (short i = 0; i < children.size(); i++) {
    if (children[i]->styles.display == Display::None) continue;
    elementsToRender.push_back(children[i].get());
  }

  std::stable_sort(
      elementsToRender.begin(), elementsToRender.end(),
      [](const Element *a, const Element *b) { return a->styles.zIndex > b->styles.zIndex; });

  return elementsToRender;
}
} // namespace Chess::Rendering
