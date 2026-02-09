#pragma once
#include "Actor.h"

class FollowActor : public Actor
{
public:
	FollowActor(GameContext* context);
	~FollowActor() = default;

	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;
private:
	class MoveComponent* mMoveComp;
	class FollowCamera* mFollowCamera;
	class MeshComponent* mMeshComp;
};