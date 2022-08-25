#pragma once


#include "Core/Base.h"

namespace kepler {

	namespace component {
		// (Im Yongsik)���ǵ� ������Ʈ ������ ��� �ĺ��� ���ΰ�??
		// ���� �ѹ��� ����ϴ°� ���� ��������(������Ʈ �ϳ� ���� ������ ������ �� ��ġ��... X)
		// ���� ���ο� ����� ã�� �������� �ӽ÷� ���� �ѹ� ����� ����մϴ�.
		constexpr int componentCount = 8;
	
		enum class eComponentType
		{
			Base = 0,
			Tag,
			Transform,

			MeshRenderer,
			SpriteRenderer,
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
			BaseFlag				= 0,
			TagFlag					= BIT_UINT64(0),
			TransformFlag			= BIT_UINT64(1),
			MeshRendererFlag		= BIT_UINT64(2),
			SpriteRendererFlag		= BIT_UINT64(3),
		};
	}
}