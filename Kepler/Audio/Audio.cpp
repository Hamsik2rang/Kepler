#include "kepch.h"

#include "Audio.h"
#include "AudioSource.hpp"

namespace kepler {

	const uint32_t				Audio::MAX_CHANNEL = 32;
	std::list<std::thread>		Audio::s_threadList;
	std::thread					Audio::s_removerThread;
	std::mutex					Audio::s_mutex;
	std::queue<std::thread::id>	Audio::s_idQueue;
	FMOD::System* Audio::s_pSystem = nullptr;
	uint32_t					Audio::s_version = 0u;
	bool						Audio::s_bIsRunning = false;

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
		for (auto& th : s_threadList)
		{
			if (th.joinable())
			{
				th.join();
			}
		}

		s_pSystem->release();
	}

	void Audio::RemoveThread()
	{
		while (s_bIsRunning)
		{
			std::thread::id removeID;
			{
				std::lock_guard<std::mutex> lock(s_mutex);

				if (s_idQueue.empty())
				{
					continue;
				}
				removeID = s_idQueue.front();
				s_idQueue.pop();
			}

			for (auto iter = s_threadList.begin(); iter != s_threadList.end(); iter++)
			{
				std::thread::id curID = iter->get_id();
				if (curID == removeID)
				{
					if (iter->joinable())
					{
						iter->join();
					}
					s_threadList.remove_if([&removeID](std::thread& th)->bool { return removeID == th.get_id(); });
					break;
				}
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

		std::lock_guard<std::mutex> lock(s_mutex);
		s_idQueue.push(std::this_thread::get_id());
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
		s_threadList.emplace_back([&source]()->void { Audio::PlayAudio(source); });
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