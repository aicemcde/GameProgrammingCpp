#include "BallActor.h"
#include "MeshComponent.h"
#include "BallMove.h"
#include "AudioComponent.h"
#include "ResourceManager.h"
#include "GameSystem.h"
#include "SoundEvent.h"

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
		SetState(EDead);
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