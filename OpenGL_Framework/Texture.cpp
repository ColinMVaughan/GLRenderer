#include "Texture.h"
#include "SOIL\SOIL.h"
//#include "stb_image.h"

#include <iostream>

Texture::~Texture()
{
	Unload();
}

bool Texture::Load(const std::string &file)
{
	TexObj = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS|SOIL_FLAG_INVERT_Y);
	
	if (TexObj == 0)
	{
		std::cout << "Texture failed to load.\n" << SOIL_last_result() << "\n";
		return false;
	}

	//Modify Texture
	glBindTexture(GL_TEXTURE_2D, TexObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //U axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //V axis

	return true;
}

bool Texture::LoadHDR(const std::string& file)
{
	//stbi_set_flip_vertically_on_load(true);
	//int width, height, nrComponents;
	//float * data = stbi_loadf(file.c_str(), &width, &height, &nrComponents, 0);
	//
	//if(data)
	//{
	//	glGenTextures(1, &TexObj);
	//	glBindTexture(GL_TEXTURE_2D, TexObj);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	std::cout << "Failed to load HDR image." << std::endl;
	//}

	return true;
}

void Texture::Unload()
{
	if (TexObj != 0)
	{
		glDeleteTextures(1, &TexObj);
		TexObj = 0;
	}
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TexObj);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}