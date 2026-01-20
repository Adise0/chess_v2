#include "Screen.h"
#include <stdexcept>
namespace Chess::Rendering {

Screen::Screen() {};
void Screen::Show() {
  if (!root) throw std::runtime_error("Screen not yet set up");
  root->styles.display = Display::Block;
}

void Screen::Hide() {
  if (!root) throw std::runtime_error("Screen not yet set up");
  root->styles.display = Display::None;
}
} // namespace Chess::Rendering
