#include "Actor.h"
#include "Math.h"
#include "GameSystem.h"


Actor::Actor(GameContext* context)
	:mGameContext(context)
	,mRecomputeWorldTransform(true)
	,mScale(1.0f)
	,mPosition(Vector3::Zero)
	,mRotation()
	,mState(EActive)
{

}

void Actor::Update(float deltaTime)
{
	if (mState == EActive)
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
	if (mState == EActive)
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