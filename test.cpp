#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  int length;
  cin >> length;
  int array[length];
  for (int i = 0; i < length; ++i ) {
    array[i] = 0;
  }

  for (int i = 0; i < length; ++i ) {
    cout<<array[i]<<",";

  }
  cout<<endl;
  return 0;
}
