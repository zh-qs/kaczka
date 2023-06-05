#include "mesh.h"

#include <fstream>

namespace kaczka
{
	Mesh::Mesh() {}

	Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<IndexType> indices) : Mesh()
	{
		vao.init();
		vao.bind();
		vbo.init();
		vbo.bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		normalVbo.init();
		normalVbo.bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		ebo.init();
		ebo.bind();

		vbo.set_static_data(&vertices[0].x, vertices.size() * sizeof(glm::vec3));
		if (!normals.empty()) normalVbo.set_static_data(&normals[0].x, normals.size() * sizeof(glm::vec3));
		ebo.set_static_data(indices.data(), indices.size() * sizeof(IndexType));

		indicesCount = indices.size();
	}

	Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<IndexType> indices, std::vector<glm::vec2> texCoords) : Mesh(vertices, normals, indices)
	{
		texVbo.init();
		texVbo.bind();
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glEnableVertexAttribArray(2);

		texVbo.set_static_data(&texCoords[0].x, texCoords.size() * sizeof(glm::vec2));
	}

	void Mesh::bindToRender() const
	{
		vao.bind();
	}
	size_t Mesh::getIndicesCount() const
	{
		return indicesCount;
	}

	Mesh Mesh::squareGridXY(int n)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<IndexType> indices;

		vertices.reserve(n * n);
		indices.reserve(6 * (n - 1) * (n - 1));

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
			{
				vertices.push_back({ -0.5f + static_cast<float>(j) / (n - 1), 0.0f, -0.5f + static_cast<float>(i) / (n - 1) });
				normals.push_back({ 0.0f,1.0f,0.0f });
			}

		for (int i = 1; i < n; ++i)
			for (int j = 1; j < n; ++j)
			{
				indices.push_back(j - 1 + i * n);
				indices.push_back(j + (i - 1) * n);
				indices.push_back(j - 1 + (i - 1) * n);

				indices.push_back(j - 1 + i * n);
				indices.push_back(j + i * n);
				indices.push_back(j + (i - 1) * n);
			}

		return { vertices, normals, indices };
	}

	Mesh Mesh::invertedCube()
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;

		vertices.reserve(8);

		vertices.push_back({ -0.5f,-0.5f,-0.5f });
		vertices.push_back({ -0.5f,-0.5f, 0.5f });
		vertices.push_back({ -0.5f,0.5f,-0.5f });
		vertices.push_back({ 0.5f,-0.5f,-0.5f });
		vertices.push_back({ -0.5f,0.5f,0.5f });
		vertices.push_back({ 0.5f,-0.5f,0.5f });
		vertices.push_back({ 0.5f,0.5f,-0.5f });
		vertices.push_back({ 0.5f,0.5f,0.5f });

		std::vector<IndexType> indices{
			1,0,2,
			2,4,1,
			6,5,7,
			5,6,3,
			0,1,3,
			5,3,1,
			6,7,4,
			4,2,6,
			2,0,3,
			3,6,2,
			5,4,7,
			4,5,1
		};

		return { vertices,normals,indices };
	}

	Mesh Mesh::loadModel(const char* filename)
	{
		std::ifstream s(filename);

		if (!s.good())
			throw std::invalid_argument("Couldn't open file");

		int vertexCount;
		s >> vertexCount;

		std::vector<glm::vec3> vertices(vertexCount);
		std::vector<glm::vec3> normals(vertexCount);
		std::vector<glm::vec2> texCoords(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			s >> vertices[i].x >> vertices[i].y >> vertices[i].z
				>> normals[i].x >> normals[i].y >> normals[i].z
				>> texCoords[i].x >> texCoords[i].y;
		}

		int triangleCount;
		s >> triangleCount;
		
		std::vector<IndexType> indices(3 * triangleCount);
		for (int i = 0; i < triangleCount; ++i)
		{
			s >> indices[3 * i] >> indices[3 * i + 1] >> indices[3 * i + 2];
		}

		return { vertices,normals,indices,texCoords };
	}

}