#include "BallActor.h"
#include "../comp/MeshComponent.h"
#include "../comp/BallMove.h"
#include "../comp/AudioComponent.h"
#include "../sys/ResourceManager.h"
#include "../sys/GameSystem.h"
#include "../util/SoundEvent.h"

BallActor::BallActor(GameContext* context)
	:Actor(context)
{
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameContext->resource->GetMesh("Assets/Sphere.gpmesh"));
	mBallMove = AddComponent_Pointer<BallMove>();
	mBallMove->SetForwardSpeed(1500.0f);
	mAudioComp = AddComponent_Pointer<AudioComponent>();
}

void BallActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mLifeSpan -= deltaTime;
	if (mLifeSpan < 0.0f)
	{
		SetState(ActorState::EDead);
	}
}

void BallActor::SetPlayer(Actor* player)
{
	mBallMove->SetPlayer(player);
}

void BallActor::HitTarget()
{
	mAudioComp->PlayEvent("event:/Ding", mGameContext);
}