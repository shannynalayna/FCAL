/* This program is just a copy of sample_1.dsl. 
You should write your second interesting FCAL
program and save it in a file with this name. */

main () { 
  int n;
  int c;

  c = 12;
  n = 1;  
  
  while ( c != 0 ) {
    n = n * c;
    c = c - 1;
  }

  print( n ); 
}



