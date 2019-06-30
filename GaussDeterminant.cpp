double GaussDeterminant(std::vector<std::vector<double>> matrix) {
  size_t size = matrix.size();
  for (size_t column = 0; column < size - 1; ++column) {
    for (size_t row = column + 1; row < size; ++row) {
      double multiplier = -matrix[row][column] / matrix[column][column];
      for (size_t tmp_col = column; tmp_col < size; ++tmp_col) {
        matrix[row][tmp_col] += matrix[column][tmp_col] * multiplier;
      }
    }
  }
  double determinant = 1;
  for (size_t i = 0; i < size; ++i) {
    determinant *= matrix[i][i];
  }
  if (determinant == -0) {
    determinant *= -1;
  }
  return determinant;
}
