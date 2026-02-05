#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <concepts>
#include "../util/state/State.h"

class StateComponent : public ComponentBase<StateComponent>
{
public:
	template<typename T, typename... Args>
		requires std::derived_from<T, class State>
	void RegisterState(Args&&... args)
	{
		auto newState = std::make_unique<T>(this, std::forward<Args>(args)...);
		mStateMap.emplace(newState->GetName(), newState.get());
		mStateUniqueMap.emplace(newState->GetName(), std::move(newState));
	}

	explicit StateComponent(class Actor* owener);
	~StateComponent() override = default;

	void Update(float deltaTime) override;
	void ProcessInput(const InputState& state) override;
	void ChangeState(const std::string& name);
	class State* GetState() const;
private:
	std::unordered_map<std::string, class State*> mStateMap;
	std::unordered_map<std::string, std::unique_ptr<class State>> mStateUniqueMap;
	class State* mCurrState = nullptr;
};