#pragma once
#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "texture.h"
#include <iostream>
#include <string>

class Texture2D : public Texture
{
public:
	Texture2D(const std::string& fileName, bool clamp = false);
	Texture2D();
	~Texture2D();
	GLubyte* data;
};
#endif //TEXTURE_2D_H