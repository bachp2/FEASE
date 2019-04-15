#pragma once
#include <stb_image.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

struct Texture{
	unsigned int tex_id;
	int width, height;
};

static void create_texture(Texture* texture, const char* filepath) {
	glGenTextures(1, &texture->tex_id);
	glBindTexture(GL_TEXTURE_2D, texture->tex_id);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int nrChannels;
	unsigned char *data = stbi_load(filepath, &texture->width, &texture->height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);
}