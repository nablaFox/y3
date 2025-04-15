#pragma once

#include "mesh.hpp"

namespace etna::engine {

constexpr float DEFAULT_SPHERE_RADIUS = 1.0f;

constexpr uint32_t DEFAULT_SPHERE_PRECISION = 200;

constexpr float DEFAULT_CUBE_SIDE = 1.0f;

constexpr float DEFAULT_PYRAMID_HEIGHT = 1.0f;

constexpr float DEFAULT_PYRAMID_SIDE_LENGTH = 1.0f;

constexpr float DEFAULT_QUAD_SIDE = 1.0f;

MeshHandle getSphere();

MeshHandle getCube();

MeshHandle getPyramid();

MeshHandle getQuad();

// TODO: implement
MeshHandle getCylinder();

// TODO: implement
MeshHandle getCone();

void initDefaultPrimitives();

void initSphere();

void initCube();

void initPyramid();

void initQuad();

}  // namespace etna::engine
