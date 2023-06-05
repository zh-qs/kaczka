#pragma once

#include <list>
#include "mesh.h"
#include "shader.h"

namespace kaczka
{
	class Renderer {
		GLuint uniformBlocks = 0;

	public:
		template <class T> 
		void addUniformBlock(const UniformBuffer<T>& ubo);
		void render(const Mesh& mesh, const Shader& shader);
		void init();
	};

	template<class T>
	inline void Renderer::addUniformBlock(const UniformBuffer<T>& ubo)
	{
		ubo.bind_to_index(uniformBlocks++);
	}
}