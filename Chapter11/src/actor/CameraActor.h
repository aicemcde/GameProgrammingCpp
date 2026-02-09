#pragma once
#include "Actor.h"
#include "../util/SoundEvent.h"

struct GameContext;

class CameraActor : public Actor
{
public:
	CameraActor(GameContext* context);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;

	void SetFootstepSurface(float value);
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class SoundEvent mFootstep;
	float mLastFootstep;
	float mFootstepSurface;
	Vector3 mCameraPos;
};