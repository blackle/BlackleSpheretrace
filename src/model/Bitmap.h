#pragma once
#include "Color.h"

class Bitmap {
public:
  Bitmap(int width, int height);
  ~Bitmap();

  Color& operator()(int i, int j);
  Color operator()(int i, int j) const;

  int width() const { return _width; }
  int height() const { return _height; }

  void diffuse_error(double intensity);

private:
  Color * _map;
  mutable Color _fill;
  int _width;
  int _height;
};

std::ostream& operator <<(std::ostream& o, const Bitmap& bmp);
