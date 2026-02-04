#pragma once
#include "Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	virtual ~CameraComponent() = default;

	void Update(float deltaTime) override;
	void ProcessInput(const InputState& state) override {};
	void OnUpdateWorldTransform(float deltaTime) override {};
protected:
	void SetViewMatrix(const Matrix4& view, const Vector3& playerPos, float deltaTime);
private:

};