#pragma once
#include <cstdint>
#include "Math.h"
#include <vector>
#include <memory>
#include <concepts>
#include <algorithm>
#include "Component.h"
#include "InputSystem.h"

class Actor
{
public:

	enum State
	{
		EActive,
		EPouse,
		EDead
	};

	template<typename T, typename... Args>
	requires std::derived_from<T, class Component>
	void AddComponent(Args&&... args)
	{
		auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
	}

	template<typename T,typename... Args>
	requires std::derived_from<T, class Component>
	T* AddComponent_Pointer(Args&&... args)
	{
		auto newComponent = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = newComponent.get();
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
		return ptr;
	}
	
	explicit Actor(class Game* game);
	virtual ~Actor()=default;

	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const InputState& state);
	virtual void ActorInput(const InputState& state);

	void ComputeWorldTransform(float deltaTime);

	State GetState() const { return mState; }
	Matrix4 GetWorldTransform() const { return mWorldTransform; }

	Quaternion GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& q) { mRotation = q; mRecomputeWorldTransform = true; }
	Vector3 GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	void SetState(const State& state) { mState = state; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	void RemoveComponent(class Component* component);
protected:
	class Game* mGame;
	State mState;

	//pos of world space
	Matrix4 mWorldTransform;

	//ComputeWorldTransform if you change value
	bool mRecomputeWorldTransform;

	std::vector<std::unique_ptr<class Component>> mComponents;

private:
	//object space
	float mScale;
	Quaternion mRotation;
	Vector3 mPosition;
};