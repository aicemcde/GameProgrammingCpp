#pragma once
#include "Actor.h"

class SplineActor : public Actor
{
public:
	SplineActor(GameContext* context);
	~SplineActor() = default;

	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;
private:
	class SplineCamera* mCameraComp;
	class MeshComponent* mMeshComp;
	class MoveComponent* mMoveComp;
};