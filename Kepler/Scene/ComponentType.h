#pragma once


#include "Core/Base.h"

namespace kepler {

	// (Im Yongsik)정의된 컴포넌트 개수를 어떻게 식별할 것인가??
	// 매직 넘버로 사용하는건 좋지 않은듯함(컴포넌트 하나 새로 정의할 때마다 수 고치고... X)
	// 추후 새로운 방법을 찾을 때까지만 임시로 매직 넘버 상수를 사용합니다.
	namespace component {
		constexpr int componentCount = 8;
	}

	enum class eComponentType
	{
		IComponent = 0,
		TagComponent,
		TransformComponent,

		MeshRendererComponent,
		SpriteRendererComponent,
	};

	enum eComponentIndex
	{
		TagIndex = 0,
		TransformIndex,
		MeshRendererIndex,
		SpriteRendererIndex,
	};

	enum eComponentFlag : uint64_t
	{
		BaseFlag = 0,
		TagFlag = BIT_UINT64(0),
		TransformFlag = BIT_UINT64(1),
		MeshRendererFlag = BIT_UINT64(2),
		SpriteRendererFlag = BIT_UINT64(3),
	};
}