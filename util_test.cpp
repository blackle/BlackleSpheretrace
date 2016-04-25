#include "util.h"
#include "bitmap.h"
#include <iostream>

double f(Point a){
  double s1 = (a-Point(-0.4,0,0)).l2() - 1;
  double s2 = (a-Point(0.4,0,0)).l2() - 1;
  return poly_min(s2,s1,0.03);
}


void render( int width, int height, Point eye, Bitmap& bmp) {
  double fov = 90;

  double factor = (double(height)/2)/tan(fov*M_PI/360.0);

  Vector lightdir = Vector(1.0,0.0,2.0);
  lightdir.normalize_l2();

  // Construct a ray for each pixel.
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      Color col(0.0,0.0,0.0);

      // Sets up ray origin and direction in view space, 
      // image plane is at z = -1.
      Vector imagePlane;

      imagePlane[0] = (-double(width)/2 + j + 0.5)/factor;
      imagePlane[1] = (-double(height)/2 + i + 0.5)/factor;
      imagePlane[2] = -1;

      // TODO: Convert ray to world space and call 
      // shadeRay(ray) to generate pixel color.  
      
      Ray ray;
      // initialize ray and convert to world coords
      ray.origin = eye;
      ray.dir = imagePlane;
      ray.dir.normalize_l2();
      ray.intsec = ray.origin;

      while(f(ray.intsec) > 0.001 and f(ray.intsec) < 1000){
        double dist = f(ray.intsec);
        ray.intsec = ray.intsec + dist*ray.dir;
        // std::cout << "marched " << dist << " units\n";
      }

      if(f(ray.intsec) < 0.001) {
        Vector normal;
        double epsi = 0.001;
        normal[0] = -(f(ray.intsec) - f(ray.intsec + epsi*STANDARD_X))/epsi;
        normal[1] = -(f(ray.intsec) - f(ray.intsec + epsi*STANDARD_Y))/epsi;
        normal[2] = -(f(ray.intsec) - f(ray.intsec + epsi*STANDARD_Z))/epsi;

        normal.normalize_l2();
        double diffuse = fmax(normal.dot(lightdir),0.0);

        Vector c = -(2.0 * normal.dot(lightdir) * normal - lightdir);

        double specular = fmax(ray.dir.dot(c),0.0);

        Vector normal2 = (2.0*normal/normal.linf() + Vector(1.0,1.0,1.0))/2.0;
        Color normalcol;
        normalcol[0] = normal2[0];
        normalcol[1] = normal2[1];
        normalcol[2] = normal2[2];


        col = diffuse*0.5 * normalcol + 
          pow(specular,50.0)*0.5 * Color(1.0,1.0,1.0);

      }

      col.clamp();
      bmp(i,j) = col;
    }
  }
}


int main(){

  int width = 500;
  int height = 500;

  Bitmap mybits(width, height);


  render( width, height, Point(0.0,0.0,2.0), mybits);
  // for(int i = 0; i < width; i++){
  //   for(int j = 0; j < height; j++){
  //     double shade1 = double(i)/width;
  //     double shade2 = double(j)/height;
  //     shade1 = (shade1 + 2.0)/4.0;
  //     shade2 = (shade2 + 2.0)/4.0;
  //     mybits(i,j) = Color(shade1*shade2, (shade1-shade2), (shade2+shade1)/2);
  //   }
  // }

  mybits.diffuse_error(2.0);

  std::cout << mybits;


  return 0;
}