#pragma once

#include <memory>
#include <vector>
#include "ignis/types.hpp"
#include "ignis/buffer.hpp"
#include "math.hpp"

namespace etna {

struct Vertex {
	alignas(16) Vec3 position;
	alignas(16) Vec3 normal;
	alignas(8) Vec2 uv;
};

typedef uint32_t Index;

class Mesh {
public:
	struct CreateInfo {
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
	};

	static std::shared_ptr<Mesh> create(const CreateInfo&);

	Mesh(const CreateInfo&);

	~Mesh();

	void update(const CreateInfo&);

	uint32_t indexCount() const;

	uint32_t vertexCount() const;

	auto getVertexBuffer() const { return m_vertexBuffer; }

	auto getIndexBuffer() const { return m_indexBuffer; }

private:
	ignis::BufferId m_vertexBuffer{IGNIS_INVALID_BUFFER_ID};
	ignis::Buffer* m_indexBuffer{nullptr};

public:
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;
};

using MeshHandle = std::shared_ptr<Mesh>;

}  // namespace etna
