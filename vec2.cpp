#include "vec2.h"

float cross(const vec2 &l, const vec2& r) {
      return l.x * r.y - r.x * l.y;
}
float dot(const vec2 &l, const vec2& r) {
      return l.x * r.x + l.y * r.y;
}



vec2 operator+(const vec2 &l, const vec2& r) {
  return {
    l.x + r.x,
    l.y + r.y
  };
}

vec2 operator-(const vec2 &l, const vec2& r) {
  return {
    l.x - r.x,
    l.y - r.y
  };
}


vec2 operator*(const vec2 &l, const float& r) {
  return {
    l.x * r,
    l.y * r
  };
}

vec2 operator*(const float& l, const vec2 &r) {
  return {
    r.x * l,
    r.y * l
  };
}

vec2 operator/(const vec2 &l, const float& r) {
  return {
    l.x / r,
    l.y / r
  };
}