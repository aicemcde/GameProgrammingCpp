#pragma once
#include <unordered_map>
#include <string>
#include "../util/SoundEvent.h"
#include "../util/Math.h"

struct GameContext;

namespace FMOD
{
	class System;
	namespace Studio
	{
		class System;
		class Bank;
		class EventDescription;
		class EventInstance;
		class Bus;
	}
}

class AudioSystem
{
public:
	AudioSystem(GameContext* context);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBank();
	class SoundEvent PlayEvent(const std::string& name);
	void SetListener(const Matrix4& viewMatrix, const Vector3& playerPos, float delatTime);

	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
	friend class SoundEvent;
private:
	void LoadBus(FMOD::Studio::Bank* bank);
	void UnloadBus(FMOD::Studio::Bank* bank);

	static unsigned int sNextID;
	GameContext* mGameContext;
	Vector3 mLastListenerPos;
	Vector3 mLastListenerVelocity;
	FMOD::Studio::System* mSystem;
	FMOD::System* mLowLevelSystem;
	std::unordered_map < std::string, FMOD::Studio::Bank* > mBanks;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstance;
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};