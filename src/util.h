
#ifndef _UTIL_
#define _UTIL_

#include <iostream>
#include <cmath>

double fmax(double a, double b, double c);
double fmin(double a, double b, double c);
double sign(double x);
double mix(double a, double b, double k);
double clamp(double k, double a, double b);
double logit(double x);
double poly_min( double a, double b, double k );
double power_min( double a, double b, double k );
double logit_min(double a, double b, double k);

enum CoordinateIndex { 
  _X_, 
  _Y_, 
  _Z_
};

struct EulerRotation {
  EulerRotation( double x, double y, double z ) : 
    x(x), y(y), z(z) {}
  double x;
  double y;
  double z;
};

class Point {
public:
  Point(double x, double y, double z);
  Point();

  Point& operator =(const Point& b);

  double& operator[](CoordinateIndex i);
  double operator[](CoordinateIndex i) const;

  Point rotate(const Point& origin, double angle, CoordinateIndex axis);

private:
  double dat_p[3];
};

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
Point operator +(const Point& u, const Vector& v); 
Vector operator -(const Point& u, const Point& v); 
Vector operator -(const Vector& u, const Vector& v); 
Vector operator -(const Vector& u); 
Point operator -(const Point& u, const Vector& v); 
Vector cross(const Vector& u, const Vector& v); 
std::ostream& operator <<(std::ostream& o, const Point& p); 
std::ostream& operator <<(std::ostream& o, const Vector& v);

const Point ORIGIN(0.0,0.0,0.0);
const Vector STANDARD_X(1.0,0.0,0.0);
const Vector STANDARD_Y(0.0,1.0,0.0);
const Vector STANDARD_Z(0.0,0.0,1.0);

class Color {
public:
  Color(); 
  Color(double r, double g, double b); 
  Color(int r, int g, int b); 
  Color(const Color& other); 

  Color& operator =(const Color& other); 
  Color operator *(const Color& other); 
  double& operator[](int i);  
  double operator[](int i) const; 
    
  void clamp();
  Color square();
  Color sqrt();

private:
  double dat_c[3];
};

Color operator *(double s, const Color& c); 
Color operator +(const Color& u, const Color& v); 
std::ostream& operator <<(std::ostream& o, const Color& c); 


struct Ray {
  Point origin;
  Vector dir;
  Point intsec;
  bool intersected;
  Vector gradient;
  double mindist;
};

#endif