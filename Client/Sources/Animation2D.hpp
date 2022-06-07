#pragma once

#include <functional>

#include "Kepler.h"

class Animation2D
{
private:
	std::vector<std::shared_ptr<kepler::ITexture2D>> m_pSprites;
	uint32_t m_frameCount;
	uint32_t m_curFrame;
	bool m_bRepeat;

public:
	Animation2D()
		: m_curFrame{ 0 }
		, m_frameCount{ 0 }
		, m_bRepeat{ false }
	{}

	Animation2D(std::initializer_list<std::shared_ptr<kepler::ITexture2D>> pSprites, uint32_t frameCount)
		: m_pSprites{ pSprites }
		, m_curFrame{ 0 }
		, m_frameCount{ frameCount }
		, m_bRepeat{ false }
	{}

	inline void Start() { m_curFrame = 0; }
	inline void Update()
	{
		m_curFrame++;
		if (m_bRepeat)
		{
			m_curFrame %= m_frameCount;
		}
		else
		{
			m_curFrame = m_curFrame < m_frameCount ? m_curFrame : m_frameCount - 1;
		}
	}
	inline void AddSprite(std::shared_ptr<kepler::ITexture2D>& pSprite) { m_pSprites.push_back(pSprite); }
	inline void AddSprites(std::initializer_list<std::shared_ptr<kepler::ITexture2D>> pSprites) { m_pSprites.insert(m_pSprites.end(), pSprites.begin(), pSprites.end()); }
	inline void SetFrameCount(uint32_t frameCount) { m_frameCount = frameCount; }
	inline void SetRepeat(bool bRepeat) { m_bRepeat = bRepeat; }
	inline std::shared_ptr<kepler::ITexture2D> GetCurFrameSprite() const { return m_pSprites[m_curFrame / (m_frameCount / m_pSprites.size())]; }
};