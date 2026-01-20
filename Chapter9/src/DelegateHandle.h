#pragma once
#include <functional>
#include <algorithm>
#include <vector>
#include <memory>

class DelegateHandle
{
public:
	DelegateHandle() = default;

	//
	DelegateHandle(std::function<void()> unSubscriber)
		: mUnSubscriber(unSubscriber) { }

	DelegateHandle(DelegateHandle&& other) noexcept
		:mUnSubscriber(std::move(other.mUnSubscriber))
	{
		other.mUnSubscriber = nullptr;
	}

	DelegateHandle& operator=(DelegateHandle&& other) noexcept
	{
		if (this != &other)
		{
			Disconnect();
			mUnSubscriber = std::move(other.mUnSubscriber);
			other.mUnSubscriber = nullptr;
		}
		return *this;
	}

	~DelegateHandle()
	{
		Disconnect();
	}

	DelegateHandle(const DelegateHandle&) = delete;
	DelegateHandle& operator=(const DelegateHandle&) = delete;
private:
	std::function<void()> mUnSubscriber;

	void Disconnect()
	{
		if (mUnSubscriber)
		{
			mUnSubscriber();
			mUnSubscriber = nullptr;
		}
	}
};