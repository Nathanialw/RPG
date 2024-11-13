#pragma once
#include <cmath>

struct i2 {
  int x;
  int y;
};

struct f2 {
  float x;
  float y;
};

f2 Normalize(f2 num) {
  float mag = std::sqrt(num.x * num.x + num.y * num.y);
  float r = 1.0f / mag;
  return {num.x * r, num.y * r};
}