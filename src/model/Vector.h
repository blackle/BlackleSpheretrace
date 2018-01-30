#pragma once
#include <iostream>
#include "CoordinateIndex.h"
#include "EulerRotation.h"

class Point;

class Vector {
public:
  Vector(double x, double y, double z);
  Vector();

  Vector& operator =(const Vector& w);

  double& operator[](CoordinateIndex i);
  double operator[](CoordinateIndex i) const;

  double l2() const; //l_2 norm
  double l1() const; //l_1 norm
  double linf() const; //l_inf norm

  void normalize_l2(); // normalize wrt l_2 norm
  void normalize_l1(); // normalize wrt l_1 norm
  void normalize_linf(); // normalize wrt l_inf norm

  double dot(const Vector& w) const;
  Vector rotate(double angle, CoordinateIndex axis);
  Vector rotate(EulerRotation rot);
  Vector cross(const Vector& w) const;

private:
  double dat_v[3];
};

Vector operator *(double s, const Vector& v);
Vector operator /(const Vector& v, double s);
Vector operator +(const Vector& u, const Vector& v); 
Vector operator -(const Point& u, const Point& v); 
Vector operator -(const Vector& u, const Vector& v); 
Vector operator -(const Vector& u); 
Vector cross(const Vector& u, const Vector& v); 

std::ostream& operator <<(std::ostream& o, const Vector& v);

const Vector STANDARD_X(1.0,0.0,0.0);
const Vector STANDARD_Y(0.0,1.0,0.0);
const Vector STANDARD_Z(0.0,0.0,1.0);
