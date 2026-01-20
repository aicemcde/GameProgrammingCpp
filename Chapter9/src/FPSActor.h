#pragma once
#include "Actor.h"

inline constexpr auto MAX_MOUSE_SPEED = 500;
inline constexpr auto MAX_MOVE_SPEED = 300.0f;
inline constexpr auto MAX_ANGULAR_SPEED = 3.1415926535f * 8.0f;

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);
	~FPSActor() = default;

	void UpdateActor(float deltaTime) override;
	void ActorInput(const InputState& state) override;
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
};