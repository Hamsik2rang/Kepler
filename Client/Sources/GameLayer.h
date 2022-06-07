#pragma once

#include "Kepler.h"
#include "Player.h"

class GameLayer : public kepler::Layer
{
private:
	std::shared_ptr<Player> m_pPlayer;
	kepler::OrthographicCamera m_camera;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnEvent(kepler::Event& e) override;
};