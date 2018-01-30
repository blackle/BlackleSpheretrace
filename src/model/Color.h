#pragma once
#include <iostream>

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
