#include "MainMenu.h"

namespace Chess::Rendering::Screens {
MainMenu::MainMenu() : Screen() {
  root = Element::GetRoot().CreateChild("MainMenu");
  root->styles.display = Display::None;

  root->styles.verticalAlignment = VerticalAlignment::Center;
  root->styles.horizontalAlignment = HorizontalAlignment::Center;

  Element *background = root->CreateChild("Background");
  background->styles.backgroundColor = {255, 255, 255, 255};
  background->styles.position = Position::Absolute;

  Element *playButton = root->CreateChild("PlayButton");
  playButton->styles.backgroundColor = {0, 0, 0, 255};
  playButton->styles.width = 300;
  playButton->styles.height = 50;
}
} // namespace Chess::Rendering::Screens
