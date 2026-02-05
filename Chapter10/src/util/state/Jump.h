#pragma once
#include "State.h"

class Jump : public StateBase<Jump>
{
public:
	explicit Jump(class StateComponent* owner);
	~Jump() override = default;

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
	void ProcessInput(const InputState& state) override;

	const char* GetName() const override
	{
		return "jump";
	}
private:
	class MoveComponent* mCachedMove = nullptr;
	float mElapsedTime = 0.0f;
	bool mIsJumping = true;
	float mDeltaTime = 0.0f;
};