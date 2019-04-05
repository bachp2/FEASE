#pragma once
#include <glad/glad.h>
#include "color.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_transform.hpp>

class Shader
{
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(){}
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);

	}


	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		Shader::reset();
		glUseProgram(ID);
	}

	inline static void reset()
	{
		glUseProgram(0);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}

	void setColor(const std::string &name, const Color &value) const
	{
		float c[] = {value.r, value.g, value.b};
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &c[0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}

	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL error: " << error << std::endl;
		}
		
	}

#define STR(x) #x

	inline static void mygl_GradientBackground( float top_r, float top_g, float top_b, float top_a,
		float bot_r, float bot_g, float bot_b, float bot_a )
	{
		glDisable(GL_DEPTH_TEST);

		static GLuint background_vao = 0;
		static GLuint background_shader = 0;

		if (background_vao == 0)
		{
			glGenVertexArrays(1, &background_vao);

			const char* vs_src = (char*) STR(
				#version 330 core
				out vec2 v_ub;
			void main()
			{
				uint idx = gl_VertexID;
				gl_Positions = vec4( idx & 1, idx >> 1, 0.0, 0.5 ) * 4.0 - 1.0;
				v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );
			}
			);

			char* fs_src = (char*) STR(
				#version 330 core
				uniform vec4 top_color;
			uniform vec4 bot_color;
			in vec2 v_uv;
			out vec4 frag_color;

			void main()
			{
				frag_color = bot_color * (1 - uv.y) + top_color * uv.y;
			}

			);
			GLuint vs_id, fs_id;
			vs_id = glCreateShader( GL_VERTEX_SHADER );
			fs_id = glCreateShader( GL_FRAGMENT_SHADER );
			glShaderSource(vs_id, 1, &vs_src, NULL);
			glShaderSource(fs_id, 1, &fs_src, NULL);
			glCompileShader(vs_id);
			glCompileShader(fs_id);
			background_shader = glCreateProgram();
			glAttachShader(background_shader, vs_id );
			glAttachShader(background_shader, fs_id );
			glLinkProgram(background_shader);

			glDeleteShader( fs_id );
			glDeleteShader( vs_id );
		}

		glUseProgram( background_shader );
		GLuint top_color_loc = glGetUniformLocation( background_shader, "top_color" );
		GLuint bot_color_loc = glGetUniformLocation( background_shader, "bot_color" );
		glUniform4f( top_color_loc, top_r, top_g, top_b, top_a );
		glUniform4f( bot_color_loc, bot_r, bot_g, bot_b, bot_a );

		glBindVertexArray( background_vao );
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};



