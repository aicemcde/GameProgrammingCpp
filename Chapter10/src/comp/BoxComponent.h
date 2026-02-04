#pragma once
#include "Component.h"
#include "../util/Collision.h"

class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* owner);
	~BoxComponent();

	void OnUpdateWorldTransform(float deltaTime) override;
	void SetObjectBox(const AABB& model) { mObjectBox = model; }
	const AABB& GetWorldBox() { return mWorldBox; }
	void SetShouldRotate(bool value) { mShouldRotate = value; }
private:
	AABB mObjectBox;
	AABB mWorldBox;
	bool mShouldRotate;
};