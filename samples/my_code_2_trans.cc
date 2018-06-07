#include <iostream>
#include "include/Matrix.h"
#include <math.h>

using namespace std;

int main ( ) {

int n;
int c;
c = 12;
n = 1;

while (c != 0)
{
n = n * c;
c = c - 1;
}
cout << n;
}
