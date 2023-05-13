#include "bezier.h"

#define CLAMP(x, y, z) (z < x ? x : (x > y ? y : z))

Bezier::Bezier(const std::vector<vec2> points) : m_points(points) {

}

vec2 Bezier::pointAt(float t) {
  t = CLAMP(0.0, 1.0, t);
  vec2 a, b;
  get(t, m_points, a, b);
  return a * (1.f - t) + b * t;
}

vec2 Bezier::derivativeAt(float t) {
  t = CLAMP(0.0, 1.0, t);
  vec2 a, b;
  get(t, m_points, a, b);
  return (b - a).normalized();
}

vec2 Bezier::normalAt(float t) {
  auto deriv = derivativeAt(t);
  return vec2(-deriv.y, deriv.x);
}

Bez Bezier::at(float t) {
  t = CLAMP(0.0, 1.0, t);
  vec2 a, b;
  get(t, m_points, a, b);
  vec2 deriv = (b - a).normalized();
  return {
    a * (1.f - t) + b * t,
    deriv,
    vec2(-deriv.y, deriv.x)
  };
}

void Bezier::get(const float& t, const std::vector<vec2>& points, vec2& p0, vec2& p1) {
  if (points.size() == 2) {
    p0 = points[0];
    p1 = points[1];
    return;
  }
  std::vector<vec2> tmp;
  for(int i = 0; i < points.size() - 1; i++) {
    const vec2& a = points[i];
    const vec2& b = points[i + 1];
    tmp.push_back(a * (1.f - t) + b * t);
  }
  get(t, tmp, p0, p1);
}
