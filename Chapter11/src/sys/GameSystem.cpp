#include "GameSystem.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "AudioSystem.h"
#include "InputSystem.h"
#include "PhysWorld.h"

GameSystem::GameSystem(class Game* game)
{
	context = std::make_unique<GameContext>();
	renderer = std::make_unique<Renderer>(context.get());
	resource = std::make_unique<ResourceManager>(context.get());
	audio = std::make_unique<AudioSystem>(context.get());
	input = std::make_unique<InputSystem>(context.get());
	physWorld = std::make_unique<PhysWorld>(context.get());
	scene = std::make_unique<Scene>(context.get());

	context->game = game;
	context->renderer = renderer.get();
	context->resource = resource.get();
	context->audio = audio.get();
	context->input = input.get();
	context->physWorld = physWorld.get();
	context->scene = scene.get();
}

GameSystem::~GameSystem() = default;