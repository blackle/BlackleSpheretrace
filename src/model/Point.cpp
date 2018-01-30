#include "Point.h"
#include "Vector.h"

Point::Point(double x, double y, double z){
  dat_p[0] = x;
  dat_p[1] = y;
  dat_p[2] = z;
}

Point::Point(){
  dat_p[0] = 0;
  dat_p[1] = 0;
  dat_p[2] = 0;
}

Point& Point::operator =(const Point& b){
  dat_p[0] = b.dat_p[0];
  dat_p[1] = b.dat_p[1];
  dat_p[2] = b.dat_p[2];
}

double& Point::operator[](CoordinateIndex i){
  return dat_p[(int)i];
}

double Point::operator[](CoordinateIndex i) const {
  return dat_p[(int)i];
}

// Point Point::rotate(const Point& origin, double angle, CoordinateIndex axis){
//   return origin + (*this-origin).rotate(angle, axis);
// }

Point operator +(const Point& u, const Vector& v){
  return Point(u[_X_]+v[_X_], u[_Y_]+v[_Y_], u[_Z_]+v[_Z_]);
}

Point operator -(const Point& u, const Vector& v){
  return Point(u[_X_]-v[_X_], u[_Y_]-v[_Y_], u[_Z_]-v[_Z_]);
}

std::ostream& operator <<(std::ostream& s, const Point& p){
  return s << "Point(" << p[_X_] << "," << p[_Y_] << "," << p[_Z_] << ")";
}
