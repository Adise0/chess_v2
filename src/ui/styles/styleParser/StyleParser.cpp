#include "StyleParser.h"
#include "../../../windowManager/WindowManager.h"
#include <algorithm>
#include <iostream>
namespace Chess::Rendering {

int StyleParser::GetStyleValue(Element *element, std::string name) {
  // #region GetStyleValue

  std::string value = element->styles.Get(name);
  try {
    short index;
    std::string_view matcher = GetMatcher(value, index);
    if (matcher == "default") return 0;
    int amount = 0;

    if (matcher == "fit-content") {
      for (short i = 0; i < element->children.size(); i++) {
        if (element->children[i]->styles.position != Position::Relative) continue;
        amount += GetStyleValue(element->children[i].get(), name);
      }
      return amount;
    }


    amount = std::max(std::stoi(value.substr(0, index)), 0);
    if (matcher == "px") return amount;
    if (matcher == "%") {
      std::string parentStr = element->parent->styles.Get(name);
      if (GetMatcher(parentStr) == "fit-content") {
        // TODO: Add warn:
        // ssize can't be a percentage of fit-content
        return 0;
      }
      int parentValue = GetStyleValue(element->parent, name);
      return (parentValue * (amount / 100.0));
    }
    if (matcher == "vw") return WindowManager::resolutionX * (amount / 100.0);
    if (matcher == "vh") return WindowManager::resolutionY * (amount / 100.0);
    throw std::runtime_error("Unknown matcher: " + std::string(matcher));
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
  if (value == "default") return "default";

  std::string_view foundMatcher;
  for (size_t i = 0; i < matchers.size(); i++) {
    size_t foundIndex = value.find(matchers[i]);
    if (foundIndex == std::string::npos) continue;
    foundMatcher = matchers[i];
    index = foundIndex;
  }

  if (foundMatcher.empty()) throw std::runtime_error("Can't find matcher in: " + value);
  return foundMatcher;
  // #endregion
}
} // namespace Chess::Rendering
