#pragma once
#include "MoveComponent.h"

class BallMove : public MoveComponent
{
public:
	explicit BallMove(class Actor* owner);
	~BallMove() = default;

	void Update(float deltaTime) override;
	void SetPlayer(class Actor* player) { mPlayer = player; }
private:
	class Actor* mPlayer = nullptr;
};