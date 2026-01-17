#include "../../element/Element.h"
#include <array>
#include <string>

namespace Chess::Rendering {

class StyleParser {
private:
  static inline std::array<std::string_view, 3> matchers{"px", "%", "fit-content"};

public:
  static int GetStyleValue(Element *element, std::string name);

  static std::string_view GetMatcher(std::string &value);
  static std::string_view GetMatcher(std::string &value, short &index);
};
} // namespace Chess::Rendering
