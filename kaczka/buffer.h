#pragma once

#include <glad/glad.h>

namespace kaczka
{
	template <typename T, GLenum TARGET>
	class GlBuffer {
		GLuint id;
	public:
		void init() {
			glGenBuffers(1, &id);
		}

		void bind() const {
			glBindBuffer(TARGET, id);
		}

		void unbind() const {
			glBindBuffer(TARGET, 0);
		}

		void set_static_data(const T* data, GLsizeiptr data_size) {
			bind();
			glBufferData(TARGET, data_size, data, GL_STATIC_DRAW);
		}

		void set_dynamic_data(const T* data, GLsizeiptr data_size) {
			bind();
			glBufferData(TARGET, data_size, data, GL_DYNAMIC_DRAW);
		}

		void set_data_with_offset(const T* data, GLintptr offset, GLsizeiptr data_size) {
			bind();
			glBufferSubData(TARGET, offset, data_size, data);
		}

		void dispose() {
			glDeleteBuffers(1, &id);
		}
	};

	using VertexBuffer = GlBuffer<GLfloat, GL_ARRAY_BUFFER>;
	using ElementBuffer = GlBuffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>;

	template <class T>
	class UniformBuffer {
		GLuint id;
		size_t count;
		static constexpr GLenum TARGET = GL_UNIFORM_BUFFER;
	public:
		void init(size_t count) {
			this->count = count;
			glGenBuffers(1, &id);
			bind();
			glBufferData(TARGET, count * sizeof(T), nullptr, GL_STATIC_DRAW);
			unbind();
		}

		void bind() const {
			glBindBuffer(TARGET, id);
		}

		void unbind() const {
			glBindBuffer(TARGET, 0);
		}

		void bind_to_index(GLuint index) const {
			glBindBufferBase(TARGET, index, id);
		}

		/*void set_static_data(const T* data, GLsizeiptr data_size) {
			glBufferData(TARGET, data_size, data, GL_STATIC_DRAW);
		}

		void set_dynamic_data(const T* data, GLsizeiptr data_size) {
			glBufferData(TARGET, data_size, data, GL_DYNAMIC_DRAW);
		}*/

		void set_data(const T* data, GLintptr idx = 0, GLsizeiptr data_count = 1) {
			bind();
			glBufferSubData(TARGET, idx * sizeof(T), data_count * sizeof(T), data);
		}

		void dispose() {
			glDeleteBuffers(1, &id);
		}
	};
}