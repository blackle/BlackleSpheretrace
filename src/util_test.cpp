#include "util.h"
#include "bitmap.h"
#include <iostream>
#include <iomanip>
#include <vector>


typedef double (*SceneObjectFunction)(Point a);
typedef void (*SceneObjectGradient)(Point a, Vector& grad);

struct SceneObject {
  SceneObjectFunction f;
  SceneObjectGradient g;
};


// double superellipse(Point a, double n, double m){
//   double xsqr = a[_X_]*a[_X_];
//   double ysqr = a[_Y_]*a[_Y_];
//   double zsqr = a[_Z_]*a[_Z_];

//   double mfrac = 1.0/m;
//   double nfrac = 1.0/n;
//   double mnfrac = m/n;

//   double xpowm = pow(xsqr, mfrac);
//   double ypowm = pow(ysqr, mfrac);
//   double zpown = pow(zsqr, nfrac);

//   double xplusy = xpowm + ypowm;

//   double func = pow(pow(xplusy, mnfrac) + zpown, n/2);

//   return func;
// }

double myfloor_f(Point a){
  double mag = 6;

  double dist_to_lumps = (1.0 - a[_Z_] +
    sin(a[_X_]*mag)*0.3 + sin(a[_Y_]*mag)*0.3)/Vector(mag,mag,-1).l2();
  double dist_to_sphere = (a - Point(4,-6,0.6)).l2() - 0.5;
  return fmin(dist_to_lumps, dist_to_sphere);
}

void myfloor_g(Point a, Vector& grad){
  double mag = 6;

  double dist_to_lumps = (1.0 - a[_Z_] +
    sin(a[_X_]*mag)*0.3 + sin(a[_Y_]*mag)*0.3)/Vector(mag,mag,-1).l2();
  double dist_to_sphere = (a - Point(4,-6,0.6)).l2() - 0.5;

  if(dist_to_lumps < dist_to_sphere){
    grad = Vector(
      mag*cos(a[_X_]*mag)*0.3 + 9*9*cos(a[_X_]*9*9)*0.005,
      mag*cos(a[_Y_]*mag)*0.3 + 9*9*cos(a[_Y_]*9*9)*0.005,
      -1.0);
  } else {
    grad = (a - Point(4,-6,0.6));
    grad.normalize_l2();
  }

}

// double f(Point a){
//   Point b = a.rotate(ORIGIN, 45, _Z_);
//   b[_X_] *= 0.8;
//   b[_Y_] *= 0.8;

//   // b[_X_] = (a[_X_]/sqrt(2) - a[_Y_]/sqrt(2))*0.8;
//   // b[_Y_] = (a[_X_]/sqrt(2) + a[_Y_]/sqrt(2))*0.8;

//   double se = superellipse(ORIGIN + (b-Point(0,0,1.0)), 0.5, 1.0) - 0.6;

//   double s1 = superellipse(ORIGIN + (a-Point(0,0,0.17)), 0.5, 1.0) - 0.4;

//   double s3 = superellipse(ORIGIN + (a-Point(0.0,0,-0.8)), 0.5, 1.0) - 0.65;


//   double s4 = superellipse(ORIGIN + (b-Point(0,0.5,-0.8)), 0.05,0.05) - 0.3;


//   double q = poly_min(s1,se,4);
//   double r = -poly_min(s4,-s3,25);

//   double spheres = poly_min(q,r,4);

//   double myfloor = 1.0 - a[_Z_];

//   return fmin(myfloor,spheres);

// }


void cast_ray( Ray& ray, std::vector<SceneObject>& scenedesc ){

  SceneObject myobj = scenedesc[0];

  double maxdist = 40;
  double epsi = 0.001;

  ray.mindist = maxdist;

  int numiters = 0;
  ray.intersected = false;

  double lambda = 0;
  double relaxation = 1.3;
  double lastdist = 0;
  double steplength = 0;
  Point sample;

  while(lambda < maxdist){

    sample = ray.origin + lambda*ray.dir;

    double dist = (*myobj.f)(sample);
    double radius = fabs(dist);

    if(dist < epsi) {
      if(dist < -epsi){
        //push out of the interior
        lambda -= fmax(0,-dist)*relaxation;
        sample = ray.origin + lambda*ray.dir;
      }
      (*myobj.g)(sample, ray.gradient);
      ray.intsec = sample;
      ray.intersected = true;
      return;
    }

    bool sorFail = relaxation > 1 &&
      (dist + lastdist) < steplength;
    if (sorFail) {
      steplength -= relaxation * steplength;
      relaxation = 1;
    } else {
      steplength = dist * relaxation;
    }
    lastdist = dist;

    lambda += steplength;

    

    // double str = pow(clamp(lambda, 0.0, 1.0),2.0);
    // double forwarddist = dist/str;
    // if(forwarddist < ray.mindist){
    //   ray.mindist = forwarddist;
    // }

    numiters++;
  }
}


void render( int width, int height, Point campos, EulerRotation camangle, std::vector<SceneObject>& scenedesc, Bitmap& bmp) {
  double fov = 90;

  double factor = (double(height)/2)/tan(fov*M_PI/360.0);

  Vector lightdir = Vector(0.0,-2.0,-1.0);
  lightdir.normalize_l2();

  // Construct a ray for each pixel.
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      Color skycol(0x9E,0xE2,0xEA);

      Color col = skycol;

      // Sets up ray origin and direction in view space, 
      // image plane is at z = -1.
      Vector imagePlane;

      imagePlane[_X_] = (-double(width)/2 + j + 0.5)/factor;
      imagePlane[_Z_] = (-double(height)/2 + i + 0.5)/factor;
      imagePlane[_Y_] = -1;

      imagePlane = imagePlane.rotate(camangle);

      // TODO: Convert ray to world space and call 
      // shadeRay(ray) to generate pixel color.  
      
      Ray ray;
      // initialize ray and convert to world coords
      ray.origin = campos;
      ray.dir = imagePlane;
      ray.dir.normalize_l2();

      cast_ray( ray, scenedesc );

      double maxdist = 40;

      double occulddist = 0.1;

      if(ray.intersected) {
        Vector normal = -ray.gradient;
        normal.normalize_l2();

        double diffuse = fmax(normal.dot(-lightdir),0.0);

        Vector c = -(2.0 * normal.dot(lightdir) * normal - lightdir);

        double specular = fmax(ray.dir.dot(c),0.0);
        specular = pow(specular,10.0);

        // only cast if diffuse is nonzero
        // if(diffuse!=0){
        //   Ray lightray;
        //   lightray.origin = ray.intsec;// - 0.1*normal;
        //   lightray.dir = lightdir;
        //   cast_ray(lightray, scenedesc);

        //   if(lightray.intersected){
        //     diffuse = 0;
        //     specular = 0;
        //   }// } else {
        //   //   double umbra = clamp(8*lightray.mindist, 0, 1);
        //   //   diffuse *= umbra;
        //   //   specular *= umbra;
        //   // }
        // }


        // Vector normal2 = (2.0*normal/normal.linf() + Vector(1.0,1.0,1.0))/2.0;
        // Color normalcol;
        // normalcol[0] = normal2[0];
        // normalcol[1] = normal2[1];
        // normalcol[2] = normal2[2];

        // double ambient = fmax(f(ray.intsec - occulddist*normal) / occulddist, 0.0);
        // ambient = 1-pow(1-ambient,2.0);

        //only apply ambient when diffuse is low
        // ambient = fmax(ambient,diffuse);

        // col = ambient*0.1*Color(0.7,0.65,0.6) + diffuse*0.8 * Color(0.7,0.65,0.6) + 
          // pow(specular,50.0)*0.5 * Color(1.0,1.0,1.0);


        col = diffuse*Color(0.6,0.6,0.6) + specular*Color(1.0,1.0,1.0);

        double fog = (ray.intsec-campos).l2()/maxdist;
        fog = 1-pow(1-fog,2.0);

        col = fog * skycol + (1-fog) * col;

      }
      double sun = pow(fmax(ray.dir.dot(lightdir),0.0),200);
      col = col + sun*Color(1.0,1.0,0.8);

      col.clamp();
      bmp(i,j) = col;

      if(j%100 == 0){
        int percent = 100.0*(1.0*i*width + j) / (height*width);
        std::cerr << "\r"<< std::setw(3) << std::setfill('0') << percent << "% completed: ";

        std::cerr  << std::string(percent/2, '|');

        std::cerr.flush();
      }
    }
  }
}


int main(){

  int width = 2000;
  int height = 2000;

  Bitmap mybits(width, height);

  std::vector<SceneObject> scenedesc;

  SceneObject myfloor;
  myfloor.f = &myfloor_f;
  myfloor.g = &myfloor_g;

  scenedesc.push_back(myfloor);

  render( width, height, Point(0.0,2.0,0.0), EulerRotation(0,0,0), scenedesc, mybits);
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