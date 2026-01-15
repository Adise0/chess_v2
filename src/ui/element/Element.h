#pragma once
#include "../styles/styles.h"
#include <memory>
#include <string>
#include <vector>

namespace Chess::Rendering {
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

public:
  Element(std::string id);
  ~Element();


  void Render();
  void AppendChild(std::unique_ptr<Element> child);
  Element *CreateChild(std::string id);

  SDL_FRect GetRect();

  short GetSiblingIndex();
  void SetSiblingIndex(short newIndex);

private:
  short GetChildIndex(Element *child);
  void SetChildIndex(Element *child, short newIndex);

  int GetLeftOffset(Element *child);
  int GetTopOffset(Element *child);
  std::unique_ptr<Element> RemoveChild(Element *child);

  SDL_Color &GetDrawColor();
  SDL_Texture *GetTexture();
  std::vector<Element *> GetRenderElements();
};


} // namespace Chess::Rendering
