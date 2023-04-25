#include "hw1.h"

namespace algebra {
bool is_zero(double x) {
  return abs(x) < 1e-3;
}

std::pair<size_t, size_t> get_shape(const Matrix& matrix) {
  size_t n = matrix.size();

  if (n == 0) {
    return {0, 0};
  }

  size_t m = matrix[0].size();
  if (m == 0) {
    throw std::invalid_argument(
        "Row number is nonzero while column number is zero.");
  }

  for (const auto& row : matrix) {
    if (row.size() != m) {
      throw std::invalid_argument("Unequal number of columns");
    }
  }

  return {n, m};
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
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  const int precision = 3;
  const int width = 8;
  std::cout << std::setprecision(precision) << std::fixed;

  std::cout << "Matrix(";
  for (int i = 0; i < n; i++) {
    if (i != 0) {
      std::cout << std::endl << std::setw(7) << "";
    }
    std::cout << "[";
    for (int j = 0; j < m; j++) {
      std::cout << std::setw(width) << std::right << matrix[i][j];
      if (j != m - 1) {
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
  auto shape = get_shape(matrix1);
  size_t n1 = shape.first, m1 = shape.second;
  shape = get_shape(matrix2);
  size_t n2 = shape.first, m2 = shape.second;

  if (m1 != n2) {
    throw std::invalid_argument(
        "Matrix 1 column number must be equal to Matrix2 row number!");
  }

  Matrix new_mat = zeros(n1, m2);
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < m2; j++) {
      for (int k = 0; k < m1; k++) {
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
  auto shape = get_shape(matrix1);
  size_t n1 = shape.first, m1 = shape.second;
  shape = get_shape(matrix2);
  size_t n2 = shape.first, m2 = shape.second;

  if (m1 != m2 || n1 != n2) {
    throw std::invalid_argument(
        "Matrix 1 must have the same shape with Matrix2!");
  }

  Matrix new_mat(matrix1);
  for (int i = 0; i < n1; i++) {
    for (int j = 0; j < m1; j++) {
      new_mat[i][j] += matrix2[i][j];
    }
  }

  return new_mat;
}

Matrix transpose(const Matrix& matrix) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  Matrix new_mat = zeros(m, n);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      new_mat[i][j] = matrix[j][i];
    }
  }

  return new_mat;
}

Matrix minor(const Matrix& matrix, size_t n, size_t m) {
  auto shape = get_shape(matrix);
  size_t r = shape.first, c = shape.second;

  if (r < n || c < m) {
    throw std::invalid_argument(
        "The given n or m is smaller than the row number or column number!");
  }

  Matrix new_mat(matrix);
  new_mat.erase(new_mat.begin() + n);
  for (auto& row : new_mat) {
    row.erase(row.begin() + m);
  }

  return new_mat;
}

double determinant(const Matrix& matrix) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (n != m) {
    throw std::logic_error(
        "A square matrix must be input to calculate the determinant!");
  }

  if (n == 0) {
    return 1.0;
  }

  if (n == 1) {
    return matrix[0][0];
  }

  double ret = 0;
  for (int i = 0; i < n; i++) {
    Matrix _minor = minor(matrix, i, 0);
    ret += (i % 2 == 0 ? 1.0 : -1.0) * matrix[i][0] * determinant(_minor);
  }

  return ret;
}

Matrix inverse(const Matrix& matrix) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (n != m) {
    throw std::logic_error("Non-square matrixs have no inverse!");
  }

  double det = determinant(matrix);
  if (is_zero(det)) {
    throw std::logic_error("Singular matrices have no inverse!");
  }

  Matrix new_mat = zeros(n, m);

  if (n == 0) {
    return new_mat;
  }

  if (n == 1) {
    new_mat[0][0] = 1.0 / matrix[0][0];
    return new_mat;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      double minor_det = determinant(minor(matrix, i, j));
      new_mat[j][i] = ((i + j) % 2 == 0 ? 1.0 : -1.0) / det * minor_det;
    }
  }

  return new_mat;
}

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis = 0) {
  auto shape = get_shape(matrix1);
  size_t n1 = shape.first, m1 = shape.second;
  shape = get_shape(matrix2);
  size_t n2 = shape.first, m2 = shape.second;

  if (axis == 0 && m1 != m2) {
    throw std::logic_error(
        "Column numbers must be the same when concatenate at axis 0!");
  }

  if (axis == 1 && n1 != n2) {
    throw std::logic_error(
        "Row numbers must be the same when concatenate at axis 0!");
  }

  if (axis == 0) {
    Matrix new_mat;
    new_mat.reserve((n1 + n2) * m1);
    for (const auto& row : matrix1) {
      new_mat.emplace_back(row);
    }
    for (const auto& row : matrix2) {
      new_mat.emplace_back(row);
    }
    return new_mat;
  } else if (axis == 1) {
    Matrix new_mat = zeros(n1, m1 + m2);
    for (int i = 0; i < n1; i++) {
      for (int j = 0; j < m1; j++) {
        new_mat[i][j] = matrix1[i][j];
      }
      for (int j = 0; j < m2; j++) {
        new_mat[i][j + m1] = matrix2[i][j];
      }
    }
    return new_mat;
  } else {
    throw std::logic_error("The concatenate axis must be in [0, 1]!");
  }
}

Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (r1 >= n || r2 >= n) {
    throw std::invalid_argument("r1 or r2 exceeds the maximum number of rows.");
  }

  Matrix new_mat(matrix);
  std::swap(new_mat[r1], new_mat[r2]);

  return new_mat;
}

Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (r >= n) {
    throw std::invalid_argument("r exceeds the maximum number of rows.");
  }

  Matrix new_mat(matrix);
  for (int i = 0; i < m; i++) {
    new_mat[r][i] *= c;
  }

  return new_mat;
}

Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (r1 >= n || r2 >= n) {
    throw std::invalid_argument("r1 or r2 exceeds the maximum number of rows.");
  }

  Matrix new_mat(matrix);
  for (int i = 0; i < m; i++) {
    new_mat[r2][i] += matrix[r1][i] * c;
  }

  return new_mat;
}

Matrix upper_triangular(const Matrix& matrix) {
  auto shape = get_shape(matrix);
  size_t n = shape.first, m = shape.second;

  if (n != m) {
    throw std::logic_error(
        "Only the square matrix can calculate the upper triangular matrix!");
  }

  Matrix new_mat(matrix);

  if (n <= 1) {
    return new_mat;
  }

  for (int j = 0; j < m - 1; j++) {
    double divisor = new_mat[j][j];
    if (is_zero(divisor)) {
      for (int i = j + 1; i < n; i++) {
        if (!is_zero(new_mat[i][j])) {
          new_mat = ero_swap(new_mat, j, i);
          break;
        }
      }
      divisor = new_mat[j][j];
    }
    for (int i = j + 1; i < n; i++) {
      double head_val = new_mat[i][j];
      if (is_zero(head_val)) {
        continue;
      }
      new_mat = ero_sum(new_mat, j, -1.0 * head_val / divisor, i);
    }
  }

  return new_mat;
}

}  // namespace algebra
