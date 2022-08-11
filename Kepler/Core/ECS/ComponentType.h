#pragma once

#include "Core/Base.h"

namespace kepler {
	namespace component {

		enum class eComponentType
		{
			None = 0,

			Tag,
			Transform,

			Camera,

			MeshRenderer,
			SpriteRenderer
		};

		enum eComponentFlag
		{
			None = 0,
			ComponentFlagTag			= BIT(0),
			ComponentFlagTransform		= BIT(1),
			ComponentFlagCamera			= BIT(2),
			ComponentFlagMeshRenderer	= BIT(3),
			ComponentFlagSpriteRenderer = BIT(4),
		};
	}
}