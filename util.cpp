#include "util.h"

double fmax(double a, double b, double c){
  return fmax(a,fmax(b,c));
}

double fmin(double a, double b, double c){
  return fmin(a,fmin(b,c));
}

double mix(double a, double b, double k){
  return a*(1-k) + b*k;
}

double clamp(double k, double a, double b){
  if(k < a) k = a;
  if(k > b) k = b;
  return k;
}

// polynomial smooth min (k = 0.1);
double poly_min( double a, double b, double k ){
    double h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

// power smooth min (k = 8);
double power_min( double a, double b, double k ){
    a = pow( a, k ); b = pow( b, k );
    return pow( (a*b)/(a+b), 1.0/k );
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

double& Point::operator[](int i){
  return dat_p[i];
}

double Point::operator[](int i) const {
  return dat_p[i];
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

double& Vector::operator[](int i){
  return dat_v[i];
}

double Vector::operator[](int i) const {
  return dat_v[i];
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

Vector Vector::cross(const Vector& other) const {
  return Vector(
      dat_v[1]*other[2] - dat_v[2]*other[1],
      dat_v[2]*other[0] - dat_v[0]*other[2],
      dat_v[0]*other[1] - dat_v[1]*other[0]);
}


Vector operator *(double s, const Vector& v){
  return Vector(s*v[0], s*v[1], s*v[2]);
}

Vector operator /(const Vector& v, double s){
  return Vector(v[0]/s, v[1]/s, v[2]/s);
}

Vector operator +(const Vector& u, const Vector& v){
  return Vector(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

Point operator +(const Point& u, const Vector& v){
  return Point(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

Vector operator -(const Point& u, const Point& v){
  return Vector(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector operator -(const Vector& u, const Vector& v){
  return Vector(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector operator -(const Vector& u){
  return Vector(-u[0], -u[1], -u[2]);
}

Point operator -(const Point& u, const Vector& v){
  return Point(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

Vector cross(const Vector& u, const Vector& v)  {
  return u.cross(v);
}

std::ostream& operator <<(std::ostream& s, const Point& p){
  return s << "Point(" << p[0] << "," << p[1] << "," << p[2] << ")";
}

std::ostream& operator <<(std::ostream& s, const Vector& v){
  return s << "Vector(" << v[0] << "," << v[1] << "," << v[2] << ")";
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
