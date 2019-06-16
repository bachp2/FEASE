#pragma once
#include <stb_image.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
extern glm::mat4 view, model, orthogonal_projection;
class Texture;
void create_texture(Texture* texture, const char* filepath, bool mipmap);
struct Texture{
	unsigned int ref = 0;
	unsigned int tex_id;
	int width, height;
	Texture(){}

	Texture(std::string filepath, bool mipmap){
		create_texture(this, filepath.c_str(), mipmap);
	}
};

class TextureQuad {
	Texture* tex = nullptr;
	unsigned int vbo, vao, ebo;
	float x, y;
	unsigned int width, height;
public:

	TextureQuad() {};

	TextureQuad(const TextureQuad &tq) {
		TextureQuad(tq.x, tq.y, tq.width, tq.height);
		this->set_texture_ptr(tq.tex);
		printf("copied!\n");
	};

	TextureQuad(int _x, int _y, unsigned int _w, unsigned int _h) : x(_x), y(_y), width(_w), height(_h) {

		const float vertices[] = {
			0, 0, 0.0f, 0, 0,
			width, height, 0.0f, 1, 1,
			0, height, 0.0f, 0, 1,
			width, 0, 0.0f, 1, 0
		};

		const unsigned int indices[] = {
			0, 1, 2, 
			0, 3, 1 
		}; 

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	};

	void set_texture_ptr(Texture* t) { tex = t; tex->ref++; }

	~TextureQuad(){
		if (!tex) return;
		if(tex->ref) {
			tex->ref--;
			return;
		}
		glDeleteTextures(1, &tex->tex_id);
		delete tex;
		tex = nullptr;

		printf("deleted!\n");
	}

	void render(Shader* s){
		if (!tex) printf("TextureQuad error: uninitialized texture\n");
		s->use();
		glm::mat4 _model = glm::mat4(1.0f);
		_model = glm::translate(_model, glm::vec3(x, y, 0));
		s->setMat4("model", _model);
		s->setMat4("projection", orthogonal_projection);
		s->setMat4("view", view);
		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, tex->tex_id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

static void create_texture(Texture* texture, const char* filepath, bool mipmap = true) {
	glGenTextures(1, &texture->tex_id);
	glBindTexture(GL_TEXTURE_2D, texture->tex_id);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// load image, create texture and generate mipmaps
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int nrChannels;
	unsigned char *data = stbi_load(filepath, &texture->width, &texture->height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if(mipmap) glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture : %s\n", filepath);
	}
	stbi_image_free(data);
}