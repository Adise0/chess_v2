#pragma once

namespace Chess::Rendering::Types {

enum class Display { Block, Flex, None };
enum class Position { Static, Relative, Absolute, Fixed, Sticky };

enum class FlexDirection { Row, Column };
enum class VerticalAlignment { Default, Top, Center, Bottom };
enum class HorizontalAlignment { Default, Left, Center, Right };
enum class Overflow { Hidden, Scroll, ScrollX, ScrollY, Show };

} // namespace Chess::Rendering::Types
