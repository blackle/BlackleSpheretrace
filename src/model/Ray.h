#pragma once
#include "Point.h"
#include "Vector.h"

struct Ray {
  Point origin;
  Vector dir;
  Point intsec;
  bool intersected;
  Vector gradient;
  double mindist;
};
