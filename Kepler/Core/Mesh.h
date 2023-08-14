#pragma once

#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"

#include <vector>
#include <string>

#include "KeplerMath.h"

namespace kepler {

class Mesh
{
private:
	std::vector<kepler::Vec4f> vertices;
	std::vector<kepler::Vec2f> uvs;



public:
	void Load(std::string filePath);

};

}