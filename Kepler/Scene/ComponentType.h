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

	// ���ǵ� ������Ʈ ������ ��� �ĺ��� ���ΰ�??
	// ���� �ѹ��� ����ϴ°� ���� ��������(������Ʈ �ϳ� ���� ������ ������ �� ��ġ��... X)
}