#include "MainMenu.h"
#include "../../styles/styles.h"
namespace Chess::Rendering::Screens {
MainMenu::MainMenu() : Screen() {

  Styles rootStyles{
      .display = Display::None,
      .width = "100vw",
      .height = "100vh",
      .backgroundColor = {255, 255, 255, 255},
  };
  root = Element::GetRoot().CreateChild("MainMenu");
  root->styles = rootStyles;


  Styles buttonStyles{
      .width = "100px",
      .height = "30px",
      .backgroundColor = {0, 0, 0, 255},
  };
  Element *playButton = root->CreateChild("PlayButton");
  playButton->styles = buttonStyles;
  // playButton->styles.width = "defualt";
  // playButton->styles.height = 50;
}
} // namespace Chess::Rendering::Screens
