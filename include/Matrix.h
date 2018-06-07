/*******************************************************************************
 * Name            : Matrix.h
 * Project         : fcal
 * Module          : matrix
 * Description     : Header file for Matrix
 * Copyright       : 2017 CSCI3081W Group WoollyMammoth. All rights reserved.
 ******************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/

class matrix {
 public:
    matrix(int i, int j);
    matrix(const matrix& m);

    int n_rows() const;
    int n_cols() const;

    float * access(const int i, const int j) const;
    friend std::ostream& operator<<(std::ostream& os, matrix& m);

    static matrix matrix_read(std::string filename);

    void operator =(const matrix& rightSide);
    matrix operator *(matrix&);

 private:
    int rows;
    int cols;
    float ** data;
};

#endif  // MATRIX_H
