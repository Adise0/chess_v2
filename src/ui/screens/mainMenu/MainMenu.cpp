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
      .width = "500px",
      .height = "30px",
      .backgroundColor = {0, 0, 0, 255},
  };
  Element *playButton = root->CreateChild("PlayButton");
  playButton->styles = buttonStyles;

  Styles buttonStyles2{
      .width = "1000px",
      .height = "30px",
      .backgroundColor = {0, 0, 255, 255},
  };
  Element *playButton2 = root->CreateChild("PlayButton2");
  playButton2->styles = buttonStyles2;
}
} // namespace Chess::Rendering::Screens
