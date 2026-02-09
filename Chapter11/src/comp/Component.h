#pragma once
#include "../util/Math.h"

struct InputState;
struct GameContext;

using ComponentID = const void*;

class Component
{
public:
	explicit Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component()=default;

	virtual void Update(float deltaTime);
	virtual void ProcessInput(const InputState& state) {};
	virtual void OnUpdateWorldTransform(float deltaTime) {};
	int GetUpdateOrder() const { return mUpdateOrder; }
	class Actor* GetOwner() const { return mOwner; }

	virtual ComponentID GetComponentID() const = 0;
protected:
	int mUpdateOrder;
	class Actor* mOwner;
};

template<class Derived>
class ComponentBase : public Component
{
public:
	explicit ComponentBase(class Actor* owner, int updateOrder = 100)
		: Component(owner, updateOrder)
	{

	}
	virtual ~ComponentBase() override = default;

	static ComponentID StaticComponentID()
	{
		static char s_ID;
		return &s_ID;
	}

	virtual ComponentID GetComponentID() const override
	{
		return StaticComponentID();
	}
};