#pragma once
#include "Actor.h"
#include "SoundEvent.h"

class CameraActor : public Actor
{
public:
	explicit CameraActor(class Game* game);

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