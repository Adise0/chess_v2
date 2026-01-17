#include "MainMenu.h"
#include "../../styles/styles.h"
namespace Chess::Rendering::Screens {
MainMenu::MainMenu() : Screen() {

  Styles rootStyles{
      .position = Position::Absolute,
      .display = Display::None,
      .width = "100vw",
      .height = "100vh",
  };
  root = Element::GetRoot().CreateChild("MainMenu");
  root->styles = rootStyles;



  // Element *playButton = root->CreateChild("PlayButton");
  // playButton->styles.backgroundColor = {0, 0, 0, 255};
  // playButton->styles.width = "defualt";
  // playButton->styles.height = 50;
}
} // namespace Chess::Rendering::Screens
