#pragma once

#include "mesh.hpp"

namespace etna::engine {

MeshHandle createSphere(float radius, uint32_t precision = 100);

MeshHandle createBrick(float width, float height, float depth);

MeshHandle createCube(float side);

MeshHandle createQuad(float width, float height);

MeshHandle createPyramid(float height, float sideLengt);

// TODO: implement
MeshHandle createCylinder(float radius, float height);

// TODO: implement
MeshHandle createCone(float radius, float height);

}  // namespace etna::engine
