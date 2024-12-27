#pragma once

#ifdef SPARKY_EMSCRIPTEN
	#define GLFW_INCLUDE_ES3
	#include <GLFW/glfw3.h>
#else
	#include <GL/glew.h>
#endif
namespace sparky { namespace graphics {

	class Buffer {
	private:

		GLuint m_BufferID;
		GLuint m_ComponentCount; // 2.0f, 2.0f, 3.0f, 6.0f

	public:

		Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
		~Buffer();

		inline GLuint getComponentCount() const {
			return m_ComponentCount;
		}
		void bind() const;
		void unbind() const;



	};


} }


