#pragma once

#include "Kepler.h"
#include <vector>

class Image
{
public:
	int width;
	int height;
	int channel;

	void* rawData;
	std::vector<kepler::Vec4f> pixels;

	void ReadFromFile(const char* filepath);
	kepler::Vec4f& GetPixel(int row, int col);

};