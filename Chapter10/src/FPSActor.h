#pragma once
#include "Actor.h"

class FPSActor : public Actor
{
public:
	FPSActor(GameContext* context);
	~FPSActor() = default;

	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;
	
	void Shoot();
private:
	void FixCollisions(float deltaTime);

	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class FPSCamera* mFPSCamera;
	class Actor* mFPSModel;
	class MeshComponent* mMeshForFPSModel;
	class SpriteComponent* mCrosshair;
	class BoxComponent* mBoxComp;
};