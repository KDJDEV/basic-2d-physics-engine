#include "matrix.h"
#include <stdexcept>
#include <iostream>

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
	mat.resize(rows);
	for (int i = 0; i < rows; i++) {
		mat[i].resize(cols);
	}
}

Matrix::Matrix(std::vector<std::vector<float>> values) : mat(values), rows(values.size()), cols(values[0].size()) {}

Matrix Matrix::identity(int n) {
	Matrix result(n, n);
	for (int i = 0; i < n; i++) {
		result.mat[i][i] = 1;
	}
	return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
	if (cols != other.rows) {
		throw std::invalid_argument("Matrix dimensions do not match for multiplication");
	}
	Matrix result(rows, other.cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < other.cols; j++) {
			for (int k = 0; k < cols; k++) {
				result.mat[i][j] += mat[i][k] * other.mat[k][j];
			}
		}
	}
	return result;
}
Matrix Matrix::operator+(const Matrix& other) const {
	if (rows != other.rows || cols != other.cols) {
		throw std::invalid_argument("Matrix dimensions do not match for addition");
	}
	Matrix result = mat;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.mat[i][j] += other.mat[i][j];
		}
	}
	return result;
}

float Matrix::determinant() const {
	if (rows != cols) {
		throw std::invalid_argument("Matrix must be square to calculate determinant");
	}
	if (rows == 1) {
		return mat[0][0];
	}
	if (rows == 2) {
		return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
	}
	float result = 0;
	for (int i = 0; i < cols; i++) {
		std::vector<std::vector<float>> submat_values(rows - 1, std::vector<float>(cols - 1));
		for (int j = 1; j < rows; j++) {
			for (int k = 0; k < cols; k++) {
				if (k < i) {
					submat_values[j - 1][k] = mat[j][k];
				}
				else if (k > i) {
					submat_values[j - 1][k - 1] = mat[j][k];
				}
			}
		}
		Matrix submat(submat_values);
		result += pow(-1, i) * mat[0][i] * submat.determinant();
	}
	return result;
}

Matrix Matrix::inverse() const {
	float det = determinant();
	if (det == 0) {
		throw std::invalid_argument("Matrix is not invertible");
	}
	Matrix result(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::vector<std::vector<float>> submat_values(rows - 1, std::vector<float>(cols - 1));
			for (int k = 0; k < rows; k++) {
				if (k == i) continue;
				for (int l = 0; l < cols; l++) {
					if (l == j) continue;
					submat_values[k < i ? k : k - 1][l < j ? l : l - 1] = mat[k][l];
				}
			}
			Matrix submat(submat_values);
			result.mat[j][i] = pow(-1, i + j) * submat.determinant() / det;
		}
	}
	return result;
}

void Matrix::print() const {
	std::cout << "[";
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << mat[i][j] << ' ';
		}
		if (i != rows - 1)
			std::cout << '\n';
	}
	std::cout << "]\n";
}