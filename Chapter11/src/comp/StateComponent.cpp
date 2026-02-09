#include "StateComponent.h"
#include "../util/state/State.h"
#include <algorithm>
#include "../util/Log.h"

StateComponent::StateComponent(Actor* owner)
	:ComponentBase(owner)
{

}

void StateComponent::Update(float deltaTime)
{
	if (mCurrState)
	{
		mCurrState->Update(deltaTime);
	}
}

void StateComponent::ProcessInput(const InputState& state)
{
	if (mCurrState)
	{
		mCurrState->ProcessInput(state);
	}
}

void StateComponent::ChangeState(const std::string& name)
{
	if (mCurrState)
	{
		mCurrState->OnExit();
	}

	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrState = iter->second;
		mCurrState->OnEnter();
		LOG_INFO("Change state: {}", name);
	}
	else
	{
		LOG_WARN("Could not find state: {}", name);
		mCurrState = nullptr;
	}
}

State* StateComponent::GetState() const
{
	if (mCurrState)
	{
		return mCurrState;
	}
	else
	{
		return nullptr;
	}
}