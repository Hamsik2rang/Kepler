#include "Image.h"
#include "stbimage/stb_image.h"


void Image::ReadFromFile(const char* filepath)
{
	rawData = stbi_load(filepath, reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height), reinterpret_cast<int*>(&channel), 0);
}
