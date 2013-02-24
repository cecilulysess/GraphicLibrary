#include "graphic_object.h"

/* The code is used to test Qimin's ReadFile code */
int main(int argc, char** argv) {
  GraphicObject* ob = new GraphicObject();
  ob->readFile();
  //ob->test();
  //vector<Property> test_v = ob->getProperties();
  //vector<ElementFace> test_f = ob->getFaces();
  //cout << "this is a vertex test: " << test_v[0].x << endl;
  //cout << "this is a face test: " << test_f[0].y << endl;
  ob->calculateVectors();
  //ob->getProperties();
  //ob->test();
  int num = ob->getPropertiesLength();
  cout << "num is: " << num; 
  float* a = new float[num*3];
  a = ob->getProperties();
  for (int i=0; i<num*3; i++) cout << a[i] << endl;
  cout << "here?" << endl;
  int num2 = ob->getFacesLength();
  cout << "face num is: " << num2;
  int* b = ob->getFaces();
  for (int i=0; i<num2*3; i++) cout << b[i] << endl;
  
  float** c = ob->getNormalization(); 
  for (int i=0; i<num; i++) cout << c[i][0] << ", " << c[i][1] << ", " << c[i][2] << endl;
  
  delete ob;
  return 0;
}
