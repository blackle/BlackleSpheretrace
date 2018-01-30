#include "util.h"
#include "isocomp.h"

struct IsoTreeNode {
  IsoTreeNode() : comp(NULL), a(NULL), b(NULL) {}
  IsoTreeNode( IsoComp* comp, IsoTreeNode* a = NULL, IsoTreeNode* b = NULL ) : 
    comp(comp), a(a), b(b) {}
  ~IsoTreeNode() { 
    if (!comp) delete comp; 
  }
  IsoComp* comp;
  IsoTreeNode* a;
  IsoTreeNode* b;
};

class FunctionEvaluator {

private:
  IsoTreeNode * _isotree_head;
};