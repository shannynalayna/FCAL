/* This program is just a copy of sample_1.dsl. You should write your FCAL program to multiply two
2-dimensional matrices and save it
in a file with this name. */

main () {
	matrix m1 = matrix_read( "./samples/my_code_1_m1.data");
	matrix m2 = matrix_read( "./samples/my_code_1_m2.data");
	matrix result = m1 * m2;
	print(result);
}



