#pragma once
#include "Actor.h"

class BallActor : public Actor
{
public:
	explicit BallActor(GameContext* context);
	~BallActor() override = default;

	void UpdateActor(float deltaTime) override;
	void SetPlayer(Actor* player);

	void HitTarget();
private:
	class AudioComponent* mAudioComp;
	class BallMove* mBallMove;
	float mLifeSpan = 2.0f;
};