#pragma once
#include "Component.h"
#include <iostream>

class MoveComponent : public ComponentBase<MoveComponent>
{
public:
	explicit MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const noexcept { return mAngularSpeed; }
	float GetForwardSpeed() const noexcept { return mForwardSpeed; }
	float GetStrafeSpeed() const noexcept { return mStrafeSpeed; }
	float GetUpSpeed() const noexcept { return mUpSpeed; }
	void SetAngularSpeed(float speed) noexcept { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) noexcept { mForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) noexcept { mStrafeSpeed = speed; }
	void SetUpSpeed(float speed) noexcept { mUpSpeed = speed; }
private:
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
	float mUpSpeed;
};