#pragma warning(disable:4996)
#include <vector>

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "shader.h"
#include "config_parser.h"

// Inspired from BennyQBD's obj loader code : https://github.com/BennyQBD/ModernOpenGLTutorial/blob/master/obj_loader.h
struct OBJIndex
{
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

	bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};

extern ConfigParser configTable;

class OBJModel
{
public:
	std::vector<OBJIndex> OBJIndices;
	std::vector<glm::vec3> vertices;

	std::vector<unsigned int> face_indices;
	std::vector<unsigned int> normal_indices;
	std::vector<unsigned int> uv_indices;
	std::vector<unsigned int> line_indices;

	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool hasUVs;
	bool hasNormals;
	unsigned int vbo, face_vao, face_ebo, line_vao, line_ebo;
	//unsigned int vbo, vao, ebo;
	OBJModel(){};
	OBJModel(const std::string& fileName);
	void render_setup();
	void render(Shader* s);

private:
	void CreateOBJFace(const std::string& line);
	void CreateOBJLine(const std::string& line);

	glm::vec2 ParseOBJVec2(const std::string& line);
	glm::vec3 ParseOBJVec3(const std::string& line);
	OBJIndex ParseOBJIndex(const std::string& token);
};


