#pragma once


#include "Core/Base.h"

namespace kepler {

	// (Im Yongsik)���ǵ� ������Ʈ ������ ��� �ĺ��� ���ΰ�??
	// ���� �ѹ��� ����ϴ°� ���� ��������(������Ʈ �ϳ� ���� ������ ������ �� ��ġ��... X)
	// ���� ���ο� ����� ã�� �������� �ӽ÷� ���� �ѹ� ����� ����մϴ�.
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