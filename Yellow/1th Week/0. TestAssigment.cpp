//
// Created by ilya on 26.08.19.
//
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

class Matrix {
 public:
  Matrix();
  Matrix(int num_rows, int num_cols);

  void Reset(int num_rows, int num_cols);

  int At(int row, int col) const;
  int& At(int row, int col);

  int GetNumRows() const;
  int GetNumColumns() const;

 private:
  int rows;
  int cols;
  std::vector<std::vector<int>> matrix;
};

Matrix::Matrix() {
  rows = 0;
  cols = 0;
}

Matrix::Matrix(int num_rows, int num_cols) {
  Reset(num_rows, num_cols);
}

void Matrix::Reset(int num_rows, int num_cols) {
  if (num_rows < 0) {
    throw out_of_range("num_rows must be >= 0");
  }
  if (num_cols < 0) {
    throw out_of_range("num_columns must be >= 0");
  }
  if (num_rows == 0 || num_cols == 0) {
    num_rows = num_cols = 0;
  }
  rows = num_rows;
  cols = num_cols;
  matrix.assign(num_rows, std::vector<int>(num_cols));
}

int Matrix::At(int row, int col) const {
  return matrix.at(row).at(col);
}

int& Matrix::At(int row, int col) {
  return matrix.at(row).at(col);
}

int Matrix::GetNumColumns() const {
  return cols;
}

int Matrix::GetNumRows() const {
  return rows;
}

std::istream& operator>>(std::istream& is, Matrix& matrix) {
  int n, m;
  is >> n >> m;
  matrix.Reset(n, m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      is >> matrix.At(i, j);
    }
  }
  return is;
}

std::ostream& operator<<(std::ostream& is, const Matrix& matrix) {
  is << matrix.GetNumRows() << " " << matrix.GetNumColumns() << std::endl;
  for (int i = 0; i < matrix.GetNumRows(); ++i) {
    for (int j = 0; j < matrix.GetNumColumns(); ++j) {
      if (j > 0) {
        is << ' ';
      }
      is << matrix.At(i, j);
    }
    is << std::endl;
  }
  return is;
}

bool operator==(const Matrix& rhs, const Matrix& lhs) {
  if (rhs.GetNumRows() != lhs.GetNumRows()) return false;
  if (rhs.GetNumColumns() != lhs.GetNumColumns()) return false;
  for (int i = 0; i < rhs.GetNumRows(); ++i) {
    for (int j = 0; j < rhs.GetNumColumns(); ++j) {
      if (rhs.At(i, j) != lhs.At(i, j)) {
        return false;
      }
    }
  }
  return true;
}

Matrix operator+(const Matrix& rhs, const Matrix& lhs) {
  if (rhs.GetNumRows() != lhs.GetNumRows()) throw std::invalid_argument("");
  if (rhs.GetNumColumns() != lhs.GetNumColumns()) throw std::invalid_argument("");

  Matrix new_matrix(rhs.GetNumRows(), rhs.GetNumColumns());
  for (int i = 0; i < rhs.GetNumRows(); ++i) {
    for (int j = 0; j < rhs.GetNumColumns(); ++j) {
      new_matrix.At(i, j) = rhs.At(i, j) + lhs.At(i, j);
    }
  }

  return new_matrix;
}

int main() {
  Matrix one;
  Matrix two;
  std::cin >> one >> two;

  std::cout << one + two << std::endl;
  one.Reset(1, 1);
  two.Reset(-1, -1);


  return 0;
}