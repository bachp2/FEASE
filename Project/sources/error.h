#pragma once
#define ERR(X) printf("Error: %s\n",X)
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

inline void print_opengl_error() {
	GLenum error = GL_NO_ERROR;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch(error)
		{
			case GL_INVALID_ENUM:
				printf("OpenGL error: %s\n", "GL_INVALID_ENUM");
				break;
			case GL_INVALID_VALUE:
				printf("OpenGL error: %s\n", "GL_INVALID_VALUE");
				break;
			case GL_INVALID_OPERATION:
				printf("OpenGL error: %s\n", "GL_INVALID_OPERATION");
				break;
			case GL_STACK_OVERFLOW:
				printf("OpenGL error: %s\n", "GL_STACK_OVERFLOW");
				break;
			case GL_STACK_UNDERFLOW:
				printf("OpenGL error: %s\n", "GL_STACK_UNDERFLOW");
				break;
			case GL_OUT_OF_MEMORY:
				printf("OpenGL error: %s\n", "GL_OUT_OF_MEMORY");
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				printf("OpenGL error: %s\n", "GL_INVALID_FRAMEBUFFER_OPERATION");
				break;
		}
	}
}