#include "hw1.h"

namespace algebra {

std::pair<size_t, size_t> get_shape(const Matrix& matrix) {
  size_t m = matrix.size();

  if (m == 0) {
    return {0, 0};
  }

  size_t n = matrix[0].size();
  if (n == 0) {
    throw std::invalid_argument(
        "Row number is nonzero while column number is zero.");
  }

  for (const auto& row : matrix) {
    if (row.size() != n) {
      throw std::invalid_argument("Unequal number of columns");
    }
  }

  return {m, n};
}

Matrix zeros(size_t n, size_t m) {
  Matrix new_mat(n, std::vector<double>(m, 0.));
  return new_mat;
}

Matrix ones(size_t n, size_t m) {
  Matrix new_mat(n, std::vector<double>(m, 1.));
  return new_mat;
}

Matrix random(size_t n, size_t m, double min, double max) {
  if (min > max) {
    std::stringstream e_msg;
    e_msg << "min (" << min << ") should be not bigger than max (" << max
          << ")!";
    throw std::invalid_argument(e_msg.str().c_str());
  }

  std::default_random_engine generator;
  std::uniform_real_distribution<> uniform_dist(min, max);

  auto dice = std::bind(uniform_dist, generator);

  Matrix new_mat;
  new_mat.reserve(n * m);
  for (int i = 0; i < n; i++) {
    new_mat.push_back(std::vector<double>());
    for (int j = 0; j < m; j++) {
      new_mat[i].push_back(dice());
    }
  }

  return new_mat;
}

void show(const Matrix& matrix) {
  size_t m, n;
  try {
    auto shape = get_shape(matrix);
    m = shape.first, n = shape.second;
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return;
  }

  const int precision = 3;
  const int width = 8;
  std::cout << std::setprecision(precision) << std::fixed;

  std::cout << "Matrix(";
  for (int i = 0; i < m; i++) {
    if (i != 0) {
      std::cout << std::endl << std::setw(7) << "";
    }
    std::cout << "[";
    for (int j = 0; j < n; j++) {
      std::cout << std::setw(width) << std::right << matrix[i][j];
      if (j != n - 1) {
        std::cout << ", ";
      }
    }
    std::cout << "]";
  }
  std::cout << ")" << std::endl;

  std::cout << std::resetiosflags(std::ios_base::fmtflags());
}

Matrix multiply(const Matrix& matrix, double c) {
  Matrix new_mat(matrix);

  for (auto& row : new_mat) {
    for (auto& elem : row) {
      elem *= c;
    }
  }

  return new_mat;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
  size_t m1, n1, m2, n2;
  try {
    auto shape = get_shape(matrix1);
    m1 = shape.first, n1 = shape.second;
  } catch (const std::exception& ex) {
    std::cerr << "Matrix1 Error: " << ex.what() << std::endl;
    return {};
  }
  try {
    auto shape = get_shape(matrix2);
    m2 = shape.first, n2 = shape.second;
  } catch (const std::exception& ex) {
    std::cerr << "Matrix2 Error: " << ex.what() << std::endl;
    return {};
  }

  if (n1 != m2) {
    throw std::invalid_argument(
        "Matrix 1 column number must be equal to Matrix2 row number!");
  }

  Matrix new_mat(m1, std::vector<double>(n2, 0.));
  for (int i = 0; i < m1; i++) {
    for (int j = 0; j < n2; j++) {
      for (int k = 0; k < n1; k++) {
        new_mat[i][j] += (matrix1[i][k] * matrix2[k][j]);
      }
    }
  }

  return new_mat;
}

Matrix sum(const Matrix& matrix, double c) {
  Matrix new_mat(matrix);

  for (auto& row : new_mat) {
    for (auto& elem : row) {
      elem += c;
    }
  }

  return new_mat;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
  size_t m1, n1, m2, n2;
  try {
    auto shape = get_shape(matrix1);
    m1 = shape.first, n1 = shape.second;
  } catch (const std::exception& ex) {
    std::cerr << "Matrix1 Error: " << ex.what() << std::endl;
    return {};
  }
  try {
    auto shape = get_shape(matrix2);
    m2 = shape.first, n2 = shape.second;
  } catch (const std::exception& ex) {
    std::cerr << "Matrix2 Error: " << ex.what() << std::endl;
    return {};
  }

  if (m1 != m2 || n1 != n2) {
    throw std::invalid_argument(
        "Matrix 1 must have the same shape with Matrix2!");
  }

  Matrix new_mat(matrix1);
  for (int i = 0; i < m1; i++) {
    for (int j = 0; j < n1; j++) {
      new_mat[i][j] += matrix2[i][j];
    }
  }

  return new_mat;
}

}  // namespace algebra
