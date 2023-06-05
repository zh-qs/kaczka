#pragma once

#include "vertex_array.h"
#include "buffer.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>

namespace kaczka
{
	class Mesh
	{
		VertexArray vao;
		VertexBuffer vbo;
		VertexBuffer normalVbo;
		VertexBuffer texVbo;
		ElementBuffer ebo;
		size_t indicesCount;

	public:
		glm::mat4 model{ 1.0f };
		glm::vec4 color = { 1.0f,0.0f,0.0f,1.0f };

		using IndexType = unsigned int;

		Mesh();
		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<IndexType> indices);
		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<IndexType> indices, std::vector<glm::vec2> texCoords);
		Mesh(const Mesh& mesh) = default;
		Mesh(Mesh&& mesh) = default;
		Mesh& operator=(const Mesh& mesh) = default;
		Mesh& operator=(Mesh&& mesh) = default;

		void bindToRender() const;
		size_t getIndicesCount() const;

		static Mesh squareGridXY(int n);
		static Mesh invertedCube();
		static Mesh loadModel(const char* filename);
	};
}