#include "PlaneActor.h"
#include "MeshComponent.h"
#include "ResourceManager.h"
#include "GameSystem.h"

PlaneActor::PlaneActor(GameContext* context)
	:Actor(context)
{
	SetScale(10.0f);
	MeshComponent* mc = AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(context->resource->GetMesh("Assets/Plane.gpmesh"));
}