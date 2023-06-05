#include "renderer.h"

namespace kaczka
{
	void Renderer::render(const Mesh& mesh, const Shader& shader)
	{
		shader.use();
		mesh.bindToRender();
		glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT, NULL);
	}

	void Renderer::init()
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}
}