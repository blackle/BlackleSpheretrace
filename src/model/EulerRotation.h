#pragma once

struct EulerRotation {
  EulerRotation( double x, double y, double z ) : 
    x(x), y(y), z(z) {}
  double x;
  double y;
  double z;
};
