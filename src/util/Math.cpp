#include "Math.h"

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
