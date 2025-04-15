#pragma once

#include <iomanip>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

namespace etna {

template <typename T, std::size_t Rows, std::size_t Cols>
class Mat {
public:
	std::array<T, Rows * Cols> elements{};

	Mat() = default;

	Mat(std::initializer_list<std::initializer_list<T>> list) {
		assert(list.size() == Rows && "Incorrect number of rows");

		auto rowIt = list.begin();

		for (std::size_t i = 0; i < Rows; i++) {
			assert(rowIt->size() == Cols && "Incorrect number of columns");

			auto colIt = rowIt->begin();

			for (std::size_t j = 0; j < Cols; j++) {
				(*this)(i, j) = *colIt;
				colIt++;
			}

			rowIt++;
		}
	}

	T& operator()(std::size_t row, std::size_t col) {
		return elements[col * Rows + row];
	}

	const T& operator()(std::size_t row, std::size_t col) const {
		return elements[col * Rows + row];
	}

	void print() const {
		for (std::size_t i = 0; i < Rows; i++) {
			std::cout << "[";
			for (std::size_t j = 0; j < Cols; j++) {
				std::cout << std::setw(8) << std::fixed << std::setprecision(4)
						  << (*this)(i, j) << " ";
			}

			std::cout << "]\n";
		}

		std::cout << std::endl;
	}

	static Mat identity() {
		static_assert(Rows == Cols, "Identity must be square");
		Mat matrix{};

		for (std::size_t i = 0; i < Rows; i++) {
			matrix(i, i) = T(1);
		}

		return matrix;
	}

	Mat<T, Cols, Rows> transpose() {
		Mat<T, Cols, Rows> transposed{};

		for (std::size_t i = 0; i < Rows; i++) {
			for (std::size_t j = 0; j < Cols; j++) {
				transposed(j, i) = (*this)(i, j);
			}
		}

		return transposed;
	}

	Mat& operator+=(const Mat& other) {
		for (std::size_t i = 0; i < Rows * Cols; i++) {
			elements[i] += other.elements[i];
		}

		return *this;
	}

	Mat& operator-=(const Mat& other) {
		for (std::size_t i = 0; i < Rows * Cols; i++) {
			elements[i] -= other.elements[i];
		}

		return *this;
	}

	Mat& operator*=(float scalar) {
		for (std::size_t i = 0; i < Rows * Cols; i++) {
			elements[i] *= scalar;
		}

		return *this;
	}

	Mat& operator/=(float scalar) {
		for (std::size_t i = 0; i < Rows * Cols; i++) {
			elements[i] /= scalar;
		}

		return *this;
	}

	Mat operator+(const Mat& other) const {
		Mat matrix = *this;

		return matrix += other;
	}

	Mat operator-(const Mat& other) const {
		Mat matrix = *this;

		return matrix -= other;
	}

	Mat operator*(float scalar) const {
		Mat matrix = *this;

		return matrix *= scalar;
	}

	Mat operator/(float scalar) const {
		Mat matrix = *this;

		return matrix /= scalar;
	}

	bool operator==(const Mat& other) const {
		for (std::size_t i = 0; i < Rows * Cols; i++) {
			if (elements[i] != other.elements[i]) {
				return false;
			}
		}

		return true;
	}
};

template <typename T, std::size_t Rows>
class Vec : public Mat<T, Rows, 1> {
public:
	Vec() = default;

	Vec(std::initializer_list<T> list) {
		assert(list.size() == Rows && "Incorrect number of elements");
		std::copy(list.begin(), list.end(), this->elements.begin());
	}

	Vec(const Mat<T, Rows, 1>& m) {
		std::copy(m.elements.begin(), m.elements.end(), this->elements.begin());
	}

	Vec(float value) {
		for (std::size_t i = 0; i < Rows; i++) {
			(*this)[i] = value;
		}
	}

	T& operator[](std::size_t row) { return (*this)(row, 0); }
	const T& operator[](std::size_t row) const { return (*this)(row, 0); }

	float length() const {
		float sum = 0;

		for (std::size_t i = 0; i < Rows; i++) {
			sum += (*this)[i] * (*this)[i];
		}

		return std::sqrt(sum);
	}

	Vec& normalize() {
		float len = length();

		if (len == 0) {
			return *this;
		}

		for (std::size_t i = 0; i < Rows; i++) {
			(*this)[i] /= len;
		}

		return *this;
	}
};

template <typename T, std::size_t Rows, std::size_t N, std::size_t Cols>
Mat<T, Rows, Cols> operator*(const Mat<T, Rows, N>& a, const Mat<T, N, Cols>& b) {
	Mat<T, Rows, Cols> result;

	for (std::size_t i = 0; i < Cols; i++) {
		for (std::size_t j = 0; j < Rows; j++) {
			T res = T(0);

			for (std::size_t k = 0; k < N; k++) {
				res += a(j, k) * b(k, i);
			}

			result(j, i) = res;
		}
	}

	return result;
}

using Mat4 = Mat<float, 4, 4>;
using Mat3 = Mat<float, 3, 3>;
using Vec4 = Vec<float, 4>;

class Vec3 : public Vec<float, 3> {
public:
	Vec3() = default;

	Vec3(float x, float y, float z) {
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}

	Vec3(const Vec<float, 3>& v) : Vec<float, 3>(v) {}

	Vec3(const Mat<float, 3, 1>& m) : Vec<float, 3>(m) {}

	Vec3 cross(const Vec3& other) const {
		return {
			(*this)[1] * other[2] - (*this)[2] * other[1],
			(*this)[2] * other[0] - (*this)[0] * other[2],
			(*this)[0] * other[1] - (*this)[1] * other[0],
		};
	}
};

using Vec2 = Vec<float, 2>;

template <typename T>
T lerp(const T& a, const T& b, float t) {
	return a + (b - a) * t;
}

template <typename T>
T square(T num) {
	return num * num;
}

}  // namespace etna
