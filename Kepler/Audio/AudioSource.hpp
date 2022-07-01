#pragma once

#include "Audio/Audio.h"

namespace kepler {

	class AudioSource
	{
	private:
		FMOD::Channel* m_pChannel;
		FMOD::Sound* m_pSound;
		bool m_bIsRepeated;
		bool m_bIsPaused;

	public:
		AudioSource()
			: m_pChannel{ nullptr }
			, m_pSound{ nullptr }
			, m_bIsRepeated{ false }
			, m_bIsPaused{ false }
		{}

		~AudioSource() = default;

		inline FMOD::Channel* GetChannel() { return m_pChannel; }
		inline FMOD::Sound* GetSound() { return m_pSound; }
		inline void SetChannel(FMOD::Channel* pChannel) { m_pChannel = pChannel; }
		inline void SetSound(FMOD::Sound* pSound) { m_pSound = pSound; }
		inline void SetRepeat(bool bIsRepeated) { m_bIsRepeated = bIsRepeated; }
		inline void SetPause(bool bIsPaused) { m_bIsPaused = bIsPaused; }
		inline bool IsRepeated() const { return m_bIsRepeated; }
		inline bool IsPaused() const { return m_bIsPaused; }
	};
}