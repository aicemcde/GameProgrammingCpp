#include "Actor.h"
#include "../util/Math.h"
#include "../sys/GameSystem.h"


Actor::Actor(GameContext* context)
	:mGameContext(context)
	,mPosition(Vector3::Zero)
{

}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::EActive)
	{
		ComputeWorldTransform(deltaTime);
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (const auto& comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{

}

void Actor::ProcessInput(const InputState& state)
{
	if (mState == ActorState::EActive)
	{
		for (const auto& comp : mComponents)
		{
			comp->ProcessInput(state);
		}
		ActorInput(state);
	}
}

void Actor::ActorInput(const InputState& state)
{

}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find_if(mComponents.begin(), mComponents.end(),
		[component](const std::unique_ptr<Component>& ptr)
		{
			return ptr.get() == component;
		});
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

Renderer* Actor::GetRenderer() noexcept
{
	return mGameContext->renderer;
}

AudioSystem* Actor::GetAudioSystem() noexcept
{
	return mGameContext->audio;
}

Scene* Actor::GetScene() noexcept
{
	return mGameContext->scene;
}

ResourceManager* Actor::GetResourceManager() noexcept
{
	return mGameContext->resource;
}

PhysWorld* Actor::GetPhysWorld() noexcept
{
	return mGameContext->physWorld;
}

GameContext* Actor::GetContext() noexcept
{
	return mGameContext;
}

void Actor::ComputeWorldTransform(float deltaTime)
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		for (const auto& comp : mComponents)
		{
			comp->OnUpdateWorldTransform(deltaTime);
		}
	}
}

void Actor::RotateToNewForward(const Vector3& forward)
{
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}