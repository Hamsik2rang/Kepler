#pragma once

#include "Kepler.h"
#include "Player.h"

class GameLayer : public kepler::Layer
{
private:
	Player* m_pPlayer;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(const float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnEvent(kepler::Event& e) override;

};