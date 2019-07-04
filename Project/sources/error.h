#pragma once
#define ERR(X) printf("Error: %s\n",X)
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

inline void print_opengl_error() {
	GLenum error = GL_NO_ERROR;
	while ((error = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL error: %d", error);
	}
}