#pragma once

#include "Kepler.h"

class Level
{
private:
	float m_width;
	float m_height;
	std::shared_ptr<kepler::ITexture2D> m_pLevelSprite;

public:
	void Init(float width, float height);
	void OnRender();
	void OnUpdate(float deltaTime);

	inline void SetWidth(float width) { m_width = width; }
	inline void SetHeight(float height) { m_height = height; }
	inline float GetWidth() const { return m_width; }
	inline float GetHeight() const { return m_height; }

};