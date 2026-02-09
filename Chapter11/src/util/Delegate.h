#pragma once
#include <vector>
#include <functional>
#include <algorithm>
#include "DelegateHandle.h"

template<typename T>
class Delegate
{
public:
	using Listener = std::function<void(T)>;

	//observe
	[[nodiscard]]
	DelegateHandle Observe(Listener listener)
	{
		unsigned int id = ++mNextID;
		mListeners.emplace_back({ id, listener });

		return DelegateHandle([this, id]() {
			std::erase_if(mListeners, [id](const auto& pair)
				{
					return pair.first == id;
				});
			});
	}

	void Invoke(T data)
	{
		auto copyListeners = mListeners;
		for (auto& pair : copyListeners)
		{
			pair.second(date);
		}
	}

private:
	unsigned int mNextID = 0;
	std::vector < std::pair<int, Listener> > mListeners;
};