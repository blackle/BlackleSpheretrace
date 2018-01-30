#include "Bitmap.h"

Bitmap::Bitmap(int width, int height){
  _width = width; _height = height;

  _map = new Color[_width*_height];
}

Bitmap::~Bitmap() {
  delete _map;
}

//automatically return a fill value of 0 if out of bounds.
Color& Bitmap::operator()(int i, int j){
  _fill[0] = 0; _fill[1] = 0; _fill[2] = 0;
  if(i < 0 or i >= _width){
    return _fill;
  }
  if(j < 0 or j >= _height){
    return _fill;
  }
  return _map[i*_height+j];
}

Color Bitmap::operator()(int i, int j) const {
  _fill[0] = 0; _fill[1] = 0; _fill[2] = 0;
  if(i < 0 or i >= _width){
    return _fill;
  }
  if(j < 0 or j >= _height){
    return _fill;
  }
  return _map[i*_height+j];
}

//diffuse error using floyd-steinberg
//intensity defines how... well... intense the dithering should be
void Bitmap::diffuse_error(double intensity){
  for(int i = 0; i < _width; i++){
    for(int _j = 0; _j < _height; _j++){
      int j = _j; int dir = 1;
      
      //serpentine order
      if(i%2 == 0){
        j = _height - _j - 1;
        dir = -1;
      }

      Color cell = (*this)(i,j);
      
      for(int c = 0; c < 3; c++){
        double chan = cell[c];
        double err = chan - int(chan*255+0.5)/255.0;

        (*this)(i,j+dir)[c] += err * 7.0/16.0 * intensity;
        (*this)(i+1,j-dir)[c] += err * 3.0/16.0 * intensity;
        (*this)(i+1,j)[c] += err * 5.0/16.0 * intensity;
        (*this)(i+1,j+dir)[c] += err * 1.0/16.0 * intensity;

      }
    }
  }
}

//output ppm format
std::ostream& operator <<(std::ostream& o, const Bitmap& bmp){
  int width = bmp.width();
  int height = bmp.height();
  o << "P3\n" << width << " " << height << "\n255\n";

  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      Color cell = bmp(i,j);
      cell.clamp();
      o << int(cell[0]*255 + 0.5) << " ";
      o << int(cell[1]*255 + 0.5) << " ";
      o << int(cell[2]*255 + 0.5) << " ";
    }
    o << "\n";
  }
}
