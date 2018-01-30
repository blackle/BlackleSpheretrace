#include <util/Math.h>
#include "Vector.h"
#include "Point.h"

Vector::Vector(double x, double y, double z){
  dat_v[0] = x;
  dat_v[1] = y;
  dat_v[2] = z;
}

Vector::Vector(){
  dat_v[0] = 0;
  dat_v[1] = 0;
  dat_v[2] = 0;
}

Vector& Vector::operator =(const Vector& w){
  dat_v[0] = w.dat_v[0];
  dat_v[1] = w.dat_v[1];
  dat_v[2] = w.dat_v[2];
}

double& Vector::operator[](CoordinateIndex i){
  return dat_v[(int)i];
}

double Vector::operator[](CoordinateIndex i) const {
  return dat_v[(int)i];
}

double Vector::l2() const {
  return sqrt(dot(*this));
}

double Vector::l1() const {
  return fabs(dat_v[0]) + fabs(dat_v[1]) + fabs(dat_v[2]);
}

double Vector::linf() const {
  return fmax(fabs(dat_v[0]), fabs(dat_v[1]), fabs(dat_v[2]));
}

void Vector::normalize_l2(){
  double size = l2();
  if(1.0 + size > 1.0){
    dat_v[0] /= size;
    dat_v[1] /= size;
    dat_v[2] /= size;
  }
}

void Vector::normalize_l1(){
  double size = l1();
  if(1.0 + size > 1.0){
    dat_v[0] /= size;
    dat_v[1] /= size;
    dat_v[2] /= size;
  }
}

void Vector::normalize_linf(){
  double size = linf();
  if(1.0 + size > 1.0){
    dat_v[0] /= size;
    dat_v[1] /= size;
    dat_v[2] /= size;
  }
}

double Vector::dot(const Vector& other) const {
  return dat_v[0]*other.dat_v[0] + 
    dat_v[1]*other.dat_v[1] + 
    dat_v[2]*other.dat_v[2];
}

Vector Vector::rotate(double angle, CoordinateIndex axis){
  double cosang = cos(angle * M_PI/180.0);
  double sinang = sin(angle * M_PI/180.0);
  if(axis == _X_){
    return Vector(
      dat_v[0],
      cosang*dat_v[1] - sinang*dat_v[2],
      sinang*dat_v[1] + cosang*dat_v[2]);
  }
  if(axis == _Y_){
    return Vector(
      cosang*dat_v[0] - sinang*dat_v[2],
      dat_v[1],
      sinang*dat_v[0] + cosang*dat_v[2]);
  }
  if(axis == _Z_){
    return Vector(
      cosang*dat_v[0] - sinang*dat_v[1],
      sinang*dat_v[0] + cosang*dat_v[1],
      dat_v[2]);
  }
}
Vector Vector::rotate(EulerRotation rot){
  return (*this).rotate(rot.x, _X_).rotate(rot.y, _Y_).rotate(rot.z, _Z_);
}

Vector Vector::cross(const Vector& other) const {
  return Vector(
      dat_v[1]*other[_Z_] - dat_v[2]*other[_Y_],
      dat_v[2]*other[_X_] - dat_v[0]*other[_Z_],
      dat_v[0]*other[_Y_] - dat_v[1]*other[_X_]);
}

Vector operator *(double s, const Vector& v){
  return Vector(s*v[_X_], s*v[_Y_], s*v[_Z_]);
}

Vector operator /(const Vector& v, double s){
  return Vector(v[_X_]/s, v[_Y_]/s, v[_Z_]/s);
}

Vector operator +(const Vector& u, const Vector& v){
  return Vector(u[_X_]+v[_X_], u[_Y_]+v[_Y_], u[_Z_]+v[_Z_]);
}

Vector operator -(const Point& u, const Point& v){
  return Vector(u[_X_]-v[_X_], u[_Y_]-v[_Y_], u[_Z_]-v[_Z_]);
}

Vector operator -(const Vector& u, const Vector& v){
  return Vector(u[_X_]-v[_X_], u[_Y_]-v[_Y_], u[_Z_]-v[_Z_]);
}

Vector operator -(const Vector& u){
  return Vector(-u[_X_], -u[_Y_], -u[_Z_]);
}

Vector cross(const Vector& u, const Vector& v)  {
  return u.cross(v);
}

std::ostream& operator <<(std::ostream& s, const Vector& v){
  return s << "Vector(" << v[_X_] << "," << v[_Y_] << "," << v[_Z_] << ")";
}

