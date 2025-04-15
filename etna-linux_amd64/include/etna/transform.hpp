#pragma once

#include "math.hpp"

namespace etna {

struct Transform {
	Vec3 position{0, 0, 0};
	float yaw{0}, pitch{0}, roll{0};
	Vec3 scale{1, 1, 1};

	static Mat4 getScaleMatrix(Vec3 scale) {
		return {
			{scale[0], 0, 0, 0},
			{0, scale[1], 0, 0},
			{0, 0, scale[2], 0},
			{0, 0, 0, 1},
		};
	}

	static Mat4 getTransMatrix(Vec3 translation) {
		return {
			{1, 0, 0, translation[0]},
			{0, 1, 0, translation[1]},
			{0, 0, 1, translation[2]},
			{0, 0, 0, 1},
		};
	}

	static Mat4 getYawMatrix(float yaw) {
		return {
			{cosf(yaw), 0, sinf(yaw), 0},
			{0, 1, 0, 0},
			{-sinf(yaw), 0, cosf(yaw), 0},
			{0, 0, 0, 1},
		};
	}

	static Mat4 getPitchMatrix(float pitch) {
		return {
			{1, 0, 0, 0},
			{0, cosf(pitch), -sinf(pitch), 0},
			{0, sinf(pitch), cosf(pitch), 0},
			{0, 0, 0, 1},
		};
	}

	static Mat4 getRollMatrix(float roll) {
		return {
			{cosf(roll), -sinf(roll), 0, 0},
			{sinf(roll), cosf(roll), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1},
		};
	}

	Mat4 getWorldMatrix() const {
		return getTransMatrix() * getScaleMatrix() * getRotMatrix();
	}

	Mat4 getTransMatrix() const { return getTransMatrix(position); }

	Mat4 getScaleMatrix() const { return getScaleMatrix(scale); }

	Mat4 getYawMatrix() const { return getYawMatrix(yaw); }

	Mat4 getPitchMatrix() const { return getPitchMatrix(pitch); }

	Mat4 getRollMatrix() const { return getRollMatrix(roll); }

	Mat4 getRotMatrix() const {
		return getPitchMatrix() * getYawMatrix() * getRollMatrix();
	}

	// TEMP: implement submatrix picking
	static Mat3 getRotMatrix3(const Mat4& M) {
		return {
			{M(0, 0), M(0, 1), M(0, 2)},
			{M(1, 0), M(1, 1), M(1, 2)},
			{M(2, 0), M(2, 1), M(2, 2)},
		};
	}

	Mat3 getRotMatrix3() const { return getRotMatrix3(getRotMatrix()); }

	Vec3 forward() const {
		Vec3 const res{
			sinf(-yaw) * cosf(pitch),
			sinf(pitch),
			cosf(-yaw) * cosf(pitch),
		};

		return res * -1;
	}

	Vec3 right() const { return forward().cross({0, 1, 0}).normalize(); }

	Vec3 up() const { return right().cross(forward()).normalize(); }
};

}  // namespace etna
