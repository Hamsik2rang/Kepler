#include "kepch.h"

#include "ECManager.hpp"

namespace kepler {
	// (Im Yongsik)TODO: 추후 정의된 컴포넌트 수를 알 수 있는 방법(ex. reflection)이 구현되면 대체할 것.
	// 그 전까진 현재 정의된 컴포넌트 타입 개수 이상만큼 직접 수정해 줘야 함.
	uint8_t ECManager::s_componentTypeCount = 16;

	ECManager::ECManager()
	{

	}

}