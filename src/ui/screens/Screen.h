#pragma once
#include "../element/Element.h"

namespace Chess::Rendering {

class Screen {
protected:
  Element *root;

public:
  Screen();

  void Show();
  void Hide();
};
} // namespace Chess::Rendering
