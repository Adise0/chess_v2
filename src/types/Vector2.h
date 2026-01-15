#pragma once
#include <cmath>


namespace Chess {

struct Vector2Int;
struct Vector2 {
  float x;
  float y;

  Vector2() : x(0), y(0) {}
  Vector2(float x, float y) : x(x), y(y) {}

  Vector2(const Vector2Int &v); // declared only

  Vector2 operator+(const Vector2 &other) const { return {x + other.x, y + other.y}; }
  Vector2 operator-(const Vector2 &other) const { return {x - other.x, y - other.y}; }
  Vector2 operator*(const Vector2 &other) const { return {x * other.x, y * other.y}; }
  Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
  Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

  static float Distance(const Vector2 &a, const Vector2 &b) {
    float x = std::pow(b.x - a.x, 2);
    float y = std::pow(b.y - a.y, 2);
    return std::sqrt(x + y);
  }
};

struct Vector2Int {
  int x;
  int y;

  Vector2Int() : x(0), y(0) {}
  Vector2Int(int x, int y) : x(x), y(y) {}

  Vector2Int(const Vector2 &v); // declared only

  Vector2Int operator+(const Vector2Int &other) const { return {x + other.x, y + other.y}; }
  Vector2Int operator-(const Vector2Int &other) const { return {x - other.x, y - other.y}; }
  Vector2Int operator*(const Vector2Int &other) const { return {x * other.x, y * other.y}; }
  Vector2Int operator*(int scalar) const { return {x * scalar, y * scalar}; }
  Vector2Int operator/(int scalar) const { return {x / scalar, y / scalar}; }
  bool operator==(const Vector2Int &other) const { return x == other.x && y == other.y; }
};

// ---- out-of-class definitions (now both types are complete) ----

inline Vector2::Vector2(const Vector2Int &v)
    : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) {}

inline Vector2Int::Vector2Int(const Vector2 &v)
    : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)) {}

} // namespace Chess
