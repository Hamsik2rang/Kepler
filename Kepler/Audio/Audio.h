#pragma once

#include "Core/Base.h"

#include <future>
#include <list>
#include <queue>
#include "fmod/inc/fmod.hpp"
// 외부 라이브러리 fmod를 사용합니다.
// fmod api documentation:
// https://www.fmod.com/resources/documentation-api


namespace kepler {

	class AudioSource;

	class Audio
	{
	private:
		static const uint32_t	MAX_CHANNEL;

		static std::list<std::future<bool>> s_futureList;
		static std::thread		s_removerThread;
		static FMOD::System*	s_pSystem;
		static uint32_t			s_version;
		static bool				s_bIsRunning;

		static void PlayAudio(AudioSource& source);
		static void RemoveThread();

	public:
		static void Init();
		static void Release();

		static void Create(AudioSource& source, const std::string& filepath);
		static void Play(AudioSource& source, bool bIsRepeat);
		static void Pause(AudioSource& source);
		static void Stop(AudioSource& source);
		static void Resume(AudioSource& source);
		static bool IsPlaying(AudioSource& source);

		static void SetVolume(AudioSource& source, float volume);
		inline static const uint32_t GetVersion() { return s_version; }
	};
}