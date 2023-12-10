//  efficient in terms of memory consumption transposition
void EffectiveTransposition(std::vector<std::vector<int>>& matrix) {
  size_t min_size = std::min(matrix.size(), matrix[0].size());
  for (size_t i = 0; i < min_size; ++i) {
    for (size_t j = i; j < min_size; ++j) {
      swap(matrix[i][j], matrix[j][i]);
    }
  }
  size_t max_size = std::max(matrix.size(), matrix[0].size());
  if (matrix.size() < matrix[0].size()) {
    matrix.resize(max_size);
    for (size_t i = min_size; i < max_size; ++i) {
      matrix[i].resize(max_size);
    }
    for (size_t i = 0; i < max_size; ++i) {
      for (size_t j = min_size; j < max_size; ++j) {
        swap(matrix[i][j], matrix[j][i]);
      }
      matrix[i].resize(min_size);
    }
  } else if (matrix.size() > matrix[0].size()) {
    for (size_t i = 0; i < min_size; ++i) {
      matrix[i].resize(max_size);
    }
    for (size_t i = min_size; i < max_size; ++i) {
      for (size_t j = 0; j < min_size; ++j) {
        swap(matrix[i][j], matrix[j][i]);
      }
    }
    matrix.resize(min_size);
  }
}
