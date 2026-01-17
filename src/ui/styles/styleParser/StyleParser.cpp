#include "StyleParser.h"
#include <algorithm>
#include <stdexcept>
namespace Chess::Rendering {

int StyleParser::GetStyleValue(Element *element, std::string name) {
  // #region GetStyleValue

  std::string value = element->styles.Get(name);
  try {
    short index;
    std::string_view matcher = GetMatcher(value, index);
    int ammount = 0;

    if (matcher == "fit-content") {
      for (short i = 0; i < element->children.size(); i++) {
        if (element->children[i]->styles.position != Position::Relative) continue;
        ammount += GetStyleValue(element->children[i].get(), name);
      }
      return ammount;
    }

    ammount = std::clamp(ammount, 0, std::stoi(value.substr(index)));
    if (matcher == "px") return ammount;
    if (matcher == "%") {

      if (GetMatcher(element->parent->styles.Get(name)) == "fit-content") {
        // TODO: Add warn:
        // ssize can't be a percentage of fit-content
        return 0;
      }
      int parentValue = GetStyleValue(element->parent, name);
      return (parentValue * (ammount / 100));
    }

  } catch (std::runtime_error e) {
    throw e;
  } catch (...) {
    throw std::runtime_error(std::string() + "invalid " + name + " for element: " + element->id +
                             " Value: " + value);
  }
  // #endregion
}

std::string_view StyleParser::GetMatcher(std::string &value) {
  // #region GetMatcher
  short _;
  return GetMatcher(value, _);
  // #endregion
}
std::string_view StyleParser::GetMatcher(std::string &value, short &index) {
  // #region GetMatcher
  std::string_view foundMatcher;
  for (size_t i = 0; i < matchers.size(); i++) {
    auto it = std::find(value.begin(), value.end(), matchers[i]);
    if (it == value.end()) continue;
    foundMatcher = matchers[i];
    index = it - value.begin();
  }

  if (foundMatcher == NULL) throw std::runtime_error("Can't find matcher in: " + value);
  return foundMatcher;
  // #endregion
}
} // namespace Chess::Rendering
