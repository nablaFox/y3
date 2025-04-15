#pragma once

#include "ignis/types.hpp"
#include "color.hpp"
#include "math.hpp"

namespace etna {

struct DirectionalLight {
	struct CreateInfo {
		std::string name;
		Vec3 direction{0, 0, -1};
		float intensity{1.f};
		Color color{WHITE};
	};

	DirectionalLight(const CreateInfo&);
	~DirectionalLight();

	ignis::BufferId getDataBuffer() const { return m_buffer; }

	Vec3 getDirection() const { return m_info.direction; }
	float getIntensity() const { return m_info.intensity; }
	std::string getName() const { return m_info.name; }
	Color getColor() const { return m_info.color; }

	void updateDirection(const Vec3&);

	void updateIntensity(float);

	void updateColor(const Color&);

	void update(const CreateInfo&);

private:
	CreateInfo m_info;

	ignis::BufferId m_buffer{IGNIS_INVALID_BUFFER_ID};

	// TEMP: will contain also info for shadows (viewproj etc.)
	struct DirectionalLightData {
		Vec3 direction;
		float intensity;
		Color color;
	};
};

}  // namespace etna
