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

public:
  Element(std::string id);
  Element(std::string id, Element *parent);
  ~Element();


  void Render();

  void AppendChild(std::unique_ptr<Element> child);
  void SetParent(Element *parent);
};


} // namespace Chess::Rendering
