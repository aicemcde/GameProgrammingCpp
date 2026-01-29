#include "GameSystem.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "AudioSystem.h"
#include "InputSystem.h"

GameSystem::GameSystem()
{
	context = std::make_unique<GameContext>();
	renderer = std::make_unique<Renderer>(context.get());
	resource = std::make_unique<ResourceManager>(context.get());
	audio = std::make_unique<AudioSystem>(context.get());
	input = std::make_unique<InputSystem>(context.get());
	scene = std::make_unique<Scene>(context.get());

	context->renderer = renderer.get();
	context->resource = resource.get();
	context->audio = audio.get();
	context->input = input.get();
	context->scene = scene.get();
}

GameSystem::~GameSystem() = default;