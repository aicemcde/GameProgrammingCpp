#pragma once
#include "Actor.h"

class PlaneActor : public Actor
{
public:
	explicit PlaneActor(GameContext* context);
	class BoxComponent* GetBoxComp() const { return mBoxComp; }
private:
	class BoxComponent* mBoxComp = nullptr;
};