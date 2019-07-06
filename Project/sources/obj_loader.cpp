#include "obj_loader.h"
#include <fstream>
#include <iostream>

#include <algorithm>
#include <map>

// Inspired from BennyQBD's obj loader code : https://github.com/BennyQBD/ModernOpenGLTutorial/blob/master/shader.cpp
static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
unsigned int ParseOBJIndexValue(const std::string& token);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

void OBJModel::render(Shader* shader)
{
	shader->use();
	shader->setMat4("model", this->Model);

	shader->setColor("color", configTable.color("line"));
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBindVertexArray(face_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawElements(GL_TRIANGLES, face_indices.size(), GL_UNSIGNED_INT, 0);

	if(!line_indices.empty())
	{
		glBindVertexArray(line_vao);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		shader->setColor("color", configTable.color("arrow_line"));
		glDrawElements(GL_LINES, line_indices.size(), GL_UNSIGNED_INT, 0);
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBindVertexArray(0);
}

void OBJModel::render_setup()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &face_vao);
	glBindVertexArray(face_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &face_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*face_indices.size(), &face_indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &line_vao);
	glBindVertexArray(line_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	if(!line_indices.empty())
	{
		glGenBuffers(1, &line_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*line_indices.size(), &line_indices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

OBJModel::OBJModel(const std::string& fileName)
{
	hasUVs = false;
	hasNormals = false;
	std::ifstream file;
	file.open(fileName.c_str());

	std::string line;
	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);

			unsigned int lineLength = line.length();

			if(lineLength < 2)
				continue;

			const char* lineCStr = line.c_str();

			switch(lineCStr[0])
			{
			case 'v':
				if(lineCStr[1] == 't')
					this->uvs.push_back(ParseOBJVec2(line));
				else if(lineCStr[1] == 'n')
					this->normals.push_back(ParseOBJVec3(line));
				else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
					this->vertices.push_back(ParseOBJVec3(line));
				break;
			case 'f':
				CreateOBJFace(line);
				break;
			case 'l':
				CreateOBJLine(line);
				break;
			default: break;
			};
		}
	}
	else
	{
		std::cerr << "Unable to load mesh: " << fileName << std::endl;
	}
	if (uvs.size() != 0) hasUVs = true;
	if (normals.size() != 0) hasNormals = true;

	for(auto& c : OBJIndices){
		face_indices.push_back(c.vertexIndex);
		normal_indices.push_back(c.normalIndex);
		uv_indices.push_back(c.uvIndex);
	}
}

void OBJModel::CreateOBJLine(const std::string& line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');
	line_indices.push_back(ParseOBJIndexValue(tokens[1]));
	line_indices.push_back(ParseOBJIndexValue(tokens[2]));
}

void OBJModel::CreateOBJFace(const std::string& line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	this->OBJIndices.push_back(ParseOBJIndex(tokens[1]));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[2]));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[3]));

	if((int)tokens.size() > 4)
	{
		this->OBJIndices.push_back(ParseOBJIndex(tokens[1]));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[3]));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[4]));
	}
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token)
{
	OBJIndex result;
	result.vertexIndex = 0;
	result.uvIndex = 0;
	result.normalIndex = 0;

	std::string buff;
	int i = 0;
	unsigned int buf[3];
	for(const char& c : token) 
	{
		if(c == '/'){
			buf[i] = ParseOBJIndexValue(buff);
			//printf("%d -- %d\n", i, buf[i]);
			i++;
			buff.clear();
		}
		else buff += c;
	}
	if(i == 3) std::cerr << "invalid face format" << std::endl;
	buf[i] = ParseOBJIndexValue(buff);

	result.vertexIndex = buf[0];
	result.uvIndex = buf[1];
	result.normalIndex = buf[2];

	return result;
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line) 
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while(vertIndexStart < tokenLength)
	{
		if(tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x,y,z);

	//glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while(vertIndexStart < tokenLength)
	{
		if(tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while(result < length)
	{
		result++;
		if(str[result] == token)
			break;
		//if(str[result-1] == token && )
	}

	return result;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token)
{
	unsigned int a = atoi(token.c_str());
	if (a == 0) return 0;
	return a - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while(end <= strLength)
	{
		while(end <= strLength)
		{
			if(cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

