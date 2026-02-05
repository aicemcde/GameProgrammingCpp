#pragma once
#include "State.h"

class Ground : public StateBase<Ground>
{
public:
	explicit Ground(class StateComponent* owner);
	~Ground() override = default;

	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "ground";
	}
};