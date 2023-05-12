#include <vector>

struct Matrix {
	std::vector<std::vector<float>> mat;
	int rows, cols;

	Matrix(int rows, int cols);
	Matrix(std::vector<std::vector<float>> values);

	static Matrix identity(int n);

	Matrix operator*(const Matrix& other) const;
	Matrix operator+(const Matrix& other) const;
	float determinant() const;

	Matrix inverse() const;
	Matrix joshInverse() const;

	void print() const;
};