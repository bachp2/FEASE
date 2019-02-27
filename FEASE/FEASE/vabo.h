#pragma once
struct VABO {
	unsigned int VAO, VBO;
	bool isInitialized = false;
};

inline static void vaboInit(VABO& vabo) {
	glGenVertexArrays(1, &vabo.VAO);
	glBindVertexArray(vabo.VAO);
	glGenBuffers(1, &vabo.VBO);
	vabo.isInitialized = true;
}

inline static void vaboDelete(VABO& vabo) {
	glDeleteVertexArrays(1, &vabo.VAO);
	glDeleteBuffers(1, &vabo.VBO);
	vabo.isInitialized = false;
}