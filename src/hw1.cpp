#include "hw1.h"

namespace algebra {
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
    std::__throw_invalid_argument(e_msg.str().c_str());
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

void show(const Matrix& matrix) {}

}  // namespace algebra
