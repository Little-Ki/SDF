#pragma once
#include <vector>

#include "vec2.h"

struct Bez {
  vec2 point;
  vec2 derivative;
  vec2 normal;
};

class Bezier {
  public:
  Bezier(const std::vector<vec2> points);

  vec2 pointAt(float t);

  vec2 normalAt(float t);

  vec2 derivativeAt(float t);

  Bez at(float t);

  private:
  
  void get(const float& t, const std::vector<vec2>& points, vec2& p0, vec2& p1);

  std::vector<vec2> m_points;
};