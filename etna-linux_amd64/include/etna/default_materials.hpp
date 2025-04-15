#pragma once

#include "color.hpp"
#include "material.hpp"

namespace etna::engine {

MaterialHandle createColorMaterial(Color color = WHITE);

MaterialHandle createPointMaterial(Color color = WHITE);

struct GridMaterialParams {
	Color color{WHITE};
	Color gridColor{BLACK};
	float gridSpacing{0.1};
	float thickness{0.005};
};

MaterialHandle createGridMaterial(GridMaterialParams);

MaterialHandle createTransparentGridMaterial(GridMaterialParams);

void initDefaultMaterials();

void initColorMaterial();

void initPointMaterial();

void initGridMaterial();

void initTransparentGridMaterial();

void initBrickOutlineMaterial();

RawShader getDefaultVertShader();

RawShader getDefaultFragShader();

RawShader getGridFragShader();

}  // namespace etna::engine
