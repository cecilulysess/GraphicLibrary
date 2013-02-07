#include "graphic_object.h"

/* The code is used to test Qimin's ReadFile code */
int main(int argc, char** argv) {
  GraphicObject* ob = new GraphicObject();
  ob->readFile();
  ob->test();
  vector<Property> test_v = ob->getProperties();
  vector<ElementFace> test_f = ob->getFaces();
  cout << "this is a vertex test: " << test_v[0].x << endl;
  cout << "this is a face test: " << test_f[0].y << endl;
  delete ob;
  return 0;
}
