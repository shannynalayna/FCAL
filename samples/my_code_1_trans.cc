#include <iostream>
#include "include/Matrix.h"
#include <math.h>
using namespace std;
int main ( ) {
  matrix m1 ( matrix::matrix_read ("./samples/my_code_1_m1.data") ) ;
  matrix m2 ( matrix::matrix_read ("./samples/my_code_1_m2.data") ) ;
  matrix result = m1 * m2;
  cout << result;
}

