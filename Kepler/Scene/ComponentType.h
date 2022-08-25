#pragma once


#include "Core/Base.h"

namespace kepler {

	enum class eComponentType
	{
		Base,
		Tag,
		Transform,

		MeshRenderer,
		SpriteRenderer,
	};

	enum eComponentIndex
	{
		Tag = 0,
		Transform,
		MeshRenderer,
		SpriteRenderer,
	};

	// 정의된 컴포넌트 개수를 어떻게 식별할 것인가??
	// 매직 넘버로 사용하는건 좋지 않은듯함(컴포넌트 하나 새로 정의할 때마다 수 고치고... X)
}