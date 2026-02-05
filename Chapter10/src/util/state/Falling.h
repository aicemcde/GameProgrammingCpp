#pragma once
#include "State.h"

class Falling : public StateBase<Falling>
{
public:
	explicit Falling(class StateComponent* owner);
	~Falling() override = default;

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "falling";
	}
private:
	class MoveComponent* mCachedMove = nullptr;
};