#pragma once
#include <iostream>
#include "CoordinateIndex.h"

class Vector;

class Point {
public:
  Point(double x, double y, double z);
  Point();

  Point& operator =(const Point& b);

  double& operator[](CoordinateIndex i);
  double operator[](CoordinateIndex i) const;

private:
  double dat_p[3];
};

Point operator +(const Point& u, const Vector& v);
Point operator -(const Point& u, const Vector& v);

std::ostream& operator <<(std::ostream& o, const Point& p);

const Point ORIGIN(0.0,0.0,0.0);
