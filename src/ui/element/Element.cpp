#include "Element.h"

namespace Chess::Rendering {


static Element &GetRoot() {
  static Element root("root", nullptr);
  return root;
}

Element::Element(std::string id) : Element(id, &Element::GetRoot()) {}

Element::Element(std::string id, Element *parent) : id(id), parent(parent) {}
} // namespace Chess::Rendering
