#include "shader_manager.h"

void ShaderManager::emplaceShader(std::string name, std::string vert_path, std::string frag_path)
{
	Shader* s = new Shader(vert_path.c_str(), frag_path.c_str());
	shaderTable[name] = s;
}

void ShaderManager::pushShader(std::string name, Shader* s)
{
	shaderTable[name] = s;
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
	for(auto& s : shaderTable){
		delete s.second;
	}
}
