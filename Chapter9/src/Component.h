#pragma once
#include <cstdint>
#include "InputSystem.h"

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component()=default;

	virtual void Update(float deltaTime);
	virtual void ProcessInput(const InputState& state) {};
	virtual void OnUpdateWorldTransform(float deltaTime) {};
	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }
protected:
	int mUpdateOrder;
	class Actor* mOwner;
};