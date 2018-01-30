#include <model/Point.h>
#include <model/Vector.h>

enum IsoCompType { 
  PRIMITIVE, 
  TRANSFORMER, 
  DISTORTER, 
  MIXER
};

class IsoComp {
public:
  virtual IsoCompType type();
};

class Primitive : public IsoComp {
public:
  virtual double f(Point a);
  virtual void g(Point a, Vector& grad);
  IsoCompType type() { return PRIMITIVE; }
};

class Transformer : public IsoComp {
public:
  virtual Point f(Point a);
  virtual void g(Point a, Vector& grad_x, Vector& grad_y, Vector& grad_z);
  IsoCompType type() { return TRANSFORMER; }
};

class Distorter : public IsoComp {
public:
  virtual double f(double a);
  virtual void g(double a, double& grad);
  IsoCompType type() { return DISTORTER; }
};

class Mixer : public IsoComp {
public:
  virtual double f(double a, double b);
  virtual double g(double a, double b, double& grad_a, double& grad_b);
  IsoCompType type() { return MIXER; }
};