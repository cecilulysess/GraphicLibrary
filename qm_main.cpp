#include "GraphicObject.h"

/* The code is used to test Qimin's ReadFile code */
int main(int argc, char** argv) {
  GraphicObject* ob = new GraphicObject();
  ob->execute();
  delete ob;
  return 0;
}
