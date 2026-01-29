#pragma once
#include <vector>
#include <memory>
#include <concepts>
#include <algorithm>
#include "Component.h"
#include "Math.h"

inline constexpr auto MAX_MOUSE_SPEED = 500;
inline constexpr auto MAX_MOVE_SPEED = 300.0f;
inline constexpr auto MAX_ANGULAR_SPEED = 3.1415926535f * 8.0f;

struct InputState;
struct GameContext;

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
		auto newComponent = std::make_unique<T>(this, std::forward<Args>(args)...);
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
	}

	template<typename T, typename... Args>
		requires std::derived_from<T, class Component>
	T* AddComponent_Pointer(Args&&... args)
	{
		auto newComponent = std::make_unique<T>(this, std::forward<Args>(args)...);
		T* ptr = newComponent.get();
		int myOrder = newComponent->GetUpdateOrder();
		auto iter = std::ranges::lower_bound(mComponents, myOrder, {}, &Component::GetUpdateOrder);
		mComponents.insert(iter, std::move(newComponent));
		return ptr;
	}

	Actor(GameContext* context);
	virtual ~Actor() = default;

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
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }

	void RemoveComponent(class Component* component);

	class Renderer* GetRenderer() noexcept;
	class AudioSystem* GetAudioSystem() noexcept;
	class Scene* GetScene() noexcept;
	class ResourceManager* GetResourceManager() noexcept;
	GameContext* GetContext() noexcept;
protected:
	GameContext* mGameContext;
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