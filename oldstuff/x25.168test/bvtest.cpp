
#include <iostream>
using namespace std;

#define _BV(x) (1 << x)
int test = 0;

#define MPIN1 0
#define MPIN2 1
#define MPIN3 2

int main()
{

  cout << (int)_BV(MPIN1) << endl;
  cout << (int)_BV(MPIN2) << endl;
  cout << (int)_BV(MPIN3) << endl;
}
