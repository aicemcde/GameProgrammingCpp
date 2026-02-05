#pragma once

struct InputState;

using StateTypeID = const void*;

class State
{
public:
	State(class StateComponent* owner)
		:mOwner(owner)
	{
		
	}
	virtual ~State() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void ProcessInput(const InputState& state) {};

	virtual StateTypeID GetStateTypeID() const = 0;

	template <typename T>
	bool IsA() const
	{
		return GetStateTypeID() == T::StaticStateTypeID();
	}

	virtual const char* GetName() const = 0;
protected:
	class StateComponent* mOwner;
};

template <typename Derived>
class StateBase : public State
{
public:
	explicit StateBase(class StateComponent* owner)
		:State(owner)
	{

	}

	~StateBase() override = default;

	static StateTypeID StaticStateTypeID()
	{
		static char s_ID;
		return &s_ID;
	}

	virtual StateTypeID GetStateTypeID() const override
	{
		return StaticStateTypeID();
	}

	template<typename T>
	T* SafeCast(State* ptr)
	{
		if (ptr && ptr->IsA<T>())
		{
			return static_cast<T>(ptr);
		}
	}
};