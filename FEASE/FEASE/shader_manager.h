#pragma once
#include <map>
#include "shader.h"
#include <string>
class ShaderManager
{
	std::map<std::string, Shader*> shaders;
public:
	ShaderManager();
	void emplaceShader(std::string name, std::string vert_path, std::string frag_path);
	void pushShader(std::string name, Shader* s);
	~ShaderManager();
};

