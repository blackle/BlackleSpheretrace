#include <cmath>
#include "Color.h"

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
