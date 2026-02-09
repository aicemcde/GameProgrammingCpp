#include "PlaneActor.h"
#include "../comp/MeshComponent.h"
#include "../sys/ResourceManager.h"
#include "../sys/GameSystem.h"
#include "../comp/BoxComponent.h"
#include "../core/Game.h"

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