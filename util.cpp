#include "util.h"

double fmax(double a, double b, double c){
  return fmax(a,fmax(b,c));
}

double fmin(double a, double b, double c){
  return fmin(a,fmin(b,c));
}

double sign(double x){
  return (x > 0) - (x < 0);
}

double mix(double a, double b, double k){
  return a*(1-k) + b*k;
}

double clamp(double k, double a, double b){
  if(k < a) k = a;
  if(k > b) k = b;
  return k;
}

double logit(double x){
  return 1.0/(1+exp(-x));
}

// polynomial smooth min (k = 0.1);
double poly_min( double a, double b, double k ){
  double h = clamp( 0.5+0.5*(b-a)*k, 0.0, 1.0 );
  return mix( b, a, h ) - h*(1.0-h)/k;
}

// power smooth min (k = 8);
double power_min( double a, double b, double k ){
  double min = fmin(a,b);
  a = a+1-min; b = b+1-min;
  a = pow( 1.0/a, k ); b = pow( 1.0/b, k );
  return 1.0/pow( a + b, 1.0/k ) - 1 + min;
}

double logit_min(double a, double b, double k){
  double decbound = logit(k*(b-a));
  return a*decbound + b*(1-decbound) - decbound*(1-decbound)/k;
}

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

Point Point::rotate(const Point& origin, double angle, CoordinateIndex axis){
  return origin + (*this-origin).rotate(angle, axis);
}

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

Point operator +(const Point& u, const Vector& v){
  return Point(u[_X_]+v[_X_], u[_Y_]+v[_Y_], u[_Z_]+v[_Z_]);
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

Point operator -(const Point& u, const Vector& v){
  return Point(u[_X_]-v[_X_], u[_Y_]-v[_Y_], u[_Z_]-v[_Z_]);
}

Vector cross(const Vector& u, const Vector& v)  {
  return u.cross(v);
}

std::ostream& operator <<(std::ostream& s, const Point& p){
  return s << "Point(" << p[_X_] << "," << p[_Y_] << "," << p[_Z_] << ")";
}

std::ostream& operator <<(std::ostream& s, const Vector& v){
  return s << "Vector(" << v[_X_] << "," << v[_Y_] << "," << v[_Z_] << ")";
}


Color::Color(){
  dat_c[0] = 0.0;
  dat_c[1] = 0.0;
  dat_c[2] = 0.0;
}

Color::Color(double r, double g, double b){ 
  dat_c[0] = r;
  dat_c[1] = g;
  dat_c[2] = b;
}

Color::Color(int r, int g, int b){ 
  dat_c[0] = r/255.0;
  dat_c[1] = g/255.0;
  dat_c[2] = b/255.0;
}

Color::Color(const Color& other){
  dat_c[0] = other.dat_c[0];
  dat_c[1] = other.dat_c[1];
  dat_c[2] = other.dat_c[2];
}

Color& Color::operator =(const Color& other){
  dat_c[0] = other.dat_c[0];
  dat_c[1] = other.dat_c[1];
  dat_c[2] = other.dat_c[2];
  return *this;
}

Color Color::operator *(const Color& other){
  return Color(dat_c[0]*other.dat_c[0], 
    dat_c[1]*other.dat_c[1], 
    dat_c[2]*other.dat_c[2]);
}

double& Color::operator[](int i){
  return dat_c[i];
}
double Color::operator[](int i) const {
  return dat_c[i];
}

void Color::clamp(){
  for (int i = 0; i < 3; i++){
    if (dat_c[i] > 1.0) dat_c[i] = 1.0; 
    if (dat_c[i] < 0.0) dat_c[i] = 0.0; 
  }
}

Color Color::square(){
  return Color(dat_c[0]*dat_c[0], 
    dat_c[1]*dat_c[1], 
    dat_c[2]*dat_c[2]);
}

Color Color::sqrt(){
  return Color(pow(dat_c[0],0.5), 
    pow(dat_c[1],0.5), 
    pow(dat_c[2],0.5));
}

Color operator *(double s, const Color& c){
  return Color(s*c[0], s*c[1], s*c[2]);
}

Color operator +(const Color& u, const Color& v){
  return Color(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

std::ostream& operator <<(std::ostream& s, const Color& c){
  return s << "Color(" << c[0] << "," << c[1] << "," << c[2] << ")";
}
