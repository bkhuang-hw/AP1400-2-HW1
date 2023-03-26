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

}  // namespace algebra
