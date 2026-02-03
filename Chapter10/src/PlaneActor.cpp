#include "PlaneActor.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
#include "GameSystem.h"
#include "BoxComponent.h"
#include "Game.h"

PlaneActor::PlaneActor(GameContext* context)
	:Actor(context)
{
	SetScale(10.0f);
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(context->resource->GetMesh("Assets/Plane.gpmesh"));
	mBoxComp = AddComponent_Pointer<BoxComponent>();
	mBoxComp->SetObjectBox(mc->GetBox());

	context->game->AddPlanes(this);
}