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
      .topMargin = "5px",
      .rightMargin = "10px",
      .bottomMargin = "10px",
      .leftMargin = "30px",
      .topPadding = "1px",
      .rightPadding = "30px",
      .bottomPadding = "10px",
      .leftPadding = "30px",
  };
  Element *playButton = root->CreateChild("PlayButton");
  playButton->styles = buttonStyles;

  Styles buttonStyles2{
      .width = "100%",
      .height = "100%",
      .backgroundColor = {0, 0, 255, 255},
  };
  Element *playButton2 = playButton->CreateChild("PlayButton2");
  playButton2->styles = buttonStyles2;

  Styles buttonStyles3{
      .width = "100%",
      .height = "100%",
      .backgroundColor = {0, 0, 255, 255},
  };
  Element *playButton3 = root->CreateChild("PlayButton3");
  playButton3->styles = buttonStyles3;
}
} // namespace Chess::Rendering::Screens
