#pragma once

#include "Core/Base.h"
#include "Entity.hpp"

namespace kepler {

class ScriptableEntity
{
private:
	Entity* m_pEntity;

protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate(float deltaTime) {}

public:
	template <typename TComponent>
	TComponent* GetComponent()
	{
		return m_pEntity->GetComponent<TComponent>();
	}
};
}