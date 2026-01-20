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


  Styles h1{
      .width = "100px",
      .height = "100px",
      .backgroundColor = {255, 0, 0, 255},
  };
  Element *h1e = root->CreateChild("h1");
  h1e->styles = h1;

  Styles h2{
      .width = "80px",
      .height = "100px",
      .backgroundColor = {255, 0, 0, 255},
  };
  Element *h2e = root->CreateChild("h2");
  h2e->styles = h2;


  Styles t1{
      .width = "100%",
      .height = "100%",
      .backgroundColor = {0, 255, 81, 255},
  };
  Element *t1e = h1e->CreateChild("t1e");
  t1e->styles = t1;


  Styles t2{
      .width = "100%",
      .height = "90%",
      .backgroundColor = {183, 0, 255, 255},
  };
  Element *t2e = h1e->CreateChild("t2e");
  t2e->styles = t2;
}
} // namespace Chess::Rendering::Screens
