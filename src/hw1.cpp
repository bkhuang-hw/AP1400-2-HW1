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

  const int precision = 4;
  const int width = 9;
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

}  // namespace algebra
