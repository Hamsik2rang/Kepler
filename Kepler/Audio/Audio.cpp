#include "kepch.h"

#include "Audio.h"
#include "AudioSource.hpp"

namespace kepler {

	const uint32_t	Audio::MAX_CHANNEL = 32;
	FMOD::System*	Audio::s_pSystem = nullptr;
	uint32_t		Audio::s_version = 0u;
	bool			Audio::s_bIsRunning = false;

	void Audio::Init()
	{
		FMOD_RESULT result = FMOD::System_Create(&s_pSystem);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to create FMOD Audio System");
			return;
		}

		result = s_pSystem->getVersion(&s_version);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to get FMOD Version");
			return;
		}

		result = s_pSystem->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to initialize Audio System");
			return;
		}

		s_bIsRunning = true;
	}

	void Audio::Release()
	{
		s_bIsRunning = false;
		s_pSystem->release();
	}


	void Audio::PlayAudio(AudioSource& source)
	{
		FMOD::Channel* pChannel = source.GetChannel();
		FMOD::Sound* pSound = source.GetSound();

		FMOD_RESULT result = s_pSystem->playSound(pSound, 0, false, &pChannel);
		source.SetChannel(pChannel);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to play Audio Source");
			return;
		}

		while (s_bIsRunning)
		{
			FMOD_RESULT result = s_pSystem->update();
			if (result != FMOD_OK)
			{
				KEPLER_CORE_ASSERT(false, "Fail to update Audio System");
				return;
			}


			if (pChannel)
			{
				bool bIsPlaying = false;
				result = pChannel->isPlaying(&bIsPlaying);
				if (!bIsPlaying)
				{
					break;
				}
			}

			if (source.IsPaused())
			{
				pChannel->setPaused(true);
			}
			else
			{
				pChannel->setPaused(false);
			}
		}
	}

	void Audio::Create(AudioSource& source, const std::string& filepath, bool bIsRepeated)
	{
		FMOD::Sound* pSound = nullptr;
		FMOD_MODE loop = bIsRepeated ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		source.SetRepeat(bIsRepeated);
		FMOD_RESULT result = s_pSystem->createSound(filepath.c_str(), loop, nullptr, &pSound);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to Create Audio Source");
		}
		source.SetSound(pSound);
	}

	void Audio::Play(AudioSource& source)
	{
		std::thread t{ [&source]()->void { Audio::PlayAudio(source); } };
		// TODO: Audio가 모든 쓰레드를 관리하는 바람직한 방법을 찾아봅시다
		t.detach();
	}

	bool Audio::IsPlaying(AudioSource& source)
	{
		bool bIsPlaying = false;
		FMOD::Channel* pChannel = source.GetChannel();
		pChannel->isPlaying(&bIsPlaying);

		return bIsPlaying;
	}

	void Audio::Pause(AudioSource& source)
	{
		source.SetPause(true);
	}

	void Audio::Resume(AudioSource& source)
	{
		source.SetPause(false);
	}

	void Audio::Stop(AudioSource& source)
	{
		FMOD::Channel* pChannel = source.GetChannel();
		pChannel->stop();
	}

	void Audio::SetVolume(AudioSource& source, float volume)
	{
		FMOD::Channel* pChannel = source.GetChannel();
		if (pChannel)
		{
			pChannel->setVolume(volume);
		}
	}
}