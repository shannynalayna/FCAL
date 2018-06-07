/*******************************************************************************
 * Name            : Matrix.cc
 * Project         : fcal
 * Module          : matrix
 * Description     : Implementation of fcal matrices in C++
 * Copyright       : 2017 CSCI3081W Group WoollyMammoth. All rights reserved.
 ******************************************************************************/

 /*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <include/Matrix.h>

 /*******************************************************************************
 * Functions
 ******************************************************************************/

  matrix::matrix(int i, int j) {
    rows = i;
    cols = j;
    data = new float * [rows];
    for (int k = 0; k < rows; k++) {
      data[k] = new float[cols];
    }
  }

  matrix::matrix(const matrix& m) {
    // Copy rows and cols
    rows = m.n_rows();
    cols = m.n_cols();
    // Allocate memory for new matrix and copy data into it
    data = new float * [rows];
    for (int k = 0; k < rows; k++) {
      data[k] = new float[cols];
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        data[i][j] = *(m.access(i, j));
      }
    }
  }

  int matrix::n_rows() const {
    return rows;
  }

  int matrix::n_cols() const {
    return cols;
  }

  float * matrix::access(const int i, const int j) const {
    return &data[i][j];
  }

  std::ostream& operator<<(std::ostream& os, matrix& m) {
    os << m.n_rows() << " " << m.n_cols() << "\n";
    for (int i = 0; i < m.n_rows(); i++) {
      for (int j = 0; j < m.n_cols(); j++) {
        os << *(m.access(i, j)) << "  ";
      }
      os << "\n";
    }
    return os;
  }

  matrix matrix::matrix_read(std::string filename) {
    // Open matrix file
    std::ifstream in(filename.c_str());
    // Get dimensions
    int rrows;
    in >> rrows;
    int rcols;
    in >> rcols;
    // Initialize new matrix
    matrix m(rrows, rcols);
    // Get matrix data
    for (int i = 0; i < rrows; i++) {
      for (int j = 0; j < rcols; j++) {
        in >> *(m.access(i, j));
      }
    }
    // Close file
    in.close();

    return m;
  }


  void matrix:: operator=(const matrix& rightSide)
  {
    if (rightSide.n_rows() == rows && rightSide.n_cols() == cols)
    {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          data[i][j] = *(rightSide.access(i, j));
        }
      }
    }
    else
      std::cout << "Error: Unable to assign matrix" << std::endl;
  }

  matrix matrix:: operator*(matrix &rightSide)
  {
    if (rightSide.n_rows() == cols) // check if able to perform multiplication
    {
      // First need to allocate new matrix to hold results
      int nRows = rows;
      int nCols = rightSide.n_cols();
      matrix nMatrix = matrix(nRows, nCols);

      //Then complete the multiplication
      for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
	nMatrix.data[i][j] = 0;
          for(int k = 0; k < cols; k++)
          {
              nMatrix.data[i][j] += data[i][k] * *(rightSide.access(k,j));
          }
        }
      }
      return nMatrix;
    }
    else
      std::cout << "Error: Unable to complete multiplication" << std::endl;
  }
