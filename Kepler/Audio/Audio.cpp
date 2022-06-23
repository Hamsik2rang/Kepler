#include "kepch.h"

#include "Audio.h"
#include "AudioSource.hpp"

namespace kepler {

	const uint32_t					Audio::MAX_CHANNEL = 32;
	std::list<std::future<bool>>	Audio::s_futureList;
	std::thread						Audio::s_removerThread;
	FMOD::System* Audio::s_pSystem = nullptr;
	uint32_t						Audio::s_version = 0u;
	bool							Audio::s_bIsRunning = false;

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
		s_removerThread = std::thread([]()->void { Audio::RemoveThread(); });
	}

	void Audio::Release()
	{
		s_bIsRunning = false;

		s_pSystem->release();
	}

	void Audio::RemoveThread()
	{
		while (s_bIsRunning)
		{
			for (auto iter = s_futureList.begin(); iter != s_futureList.end(); iter++)
			{
				//ぬぬぬぬ
				//std::future_status status = iter->wait_for(std::chrono::milliseconds(3));
				//if (status == std::future_status::ready)
				//{
				//	bool result = iter->get();
				//	std::swap(*iter, s_threadList.back());
				//	s_threadList.pop_back();
				//	break;
				//}
				s_futureList.remove_if([](std::future<bool>& f)->bool
					{
						bool result = false;
						std::future_status status = f.wait_for(std::chrono::milliseconds(3));
						if (status == std::future_status::ready)
						{
							result = f.get();
						}
						return result;
					});
			}
		}
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

	void Audio::Create(AudioSource& source, const std::string& filepath)
	{
		FMOD::Sound* pSound = nullptr;
		FMOD_MODE loop = source.IsRepeated() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		FMOD_RESULT result = s_pSystem->createSound(filepath.c_str(), loop, nullptr, &pSound);
		if (result != FMOD_OK)
		{
			KEPLER_CORE_ASSERT(false, "Fail to Create Audio Source");
		}
		source.SetSound(pSound);
	}

	void Audio::Play(AudioSource& source, bool bIsRepeat)
	{
		source.SetRepeat(bIsRepeat);
		s_futureList.emplace_back(std::async([&source]()->bool { Audio::PlayAudio(source); return true; }));
		KEPLER_CORE_INFO("Current AudioSource List Size: {0}", s_futureList.size());
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