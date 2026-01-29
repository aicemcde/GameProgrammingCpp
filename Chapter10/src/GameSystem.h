#pragma once
#include <memory>

class Scene;
class ResourceManager;
class AudioSystem;
class InputSystem;
class Renderer;

struct GameContext
{
	class Scene* scene = nullptr;
	class ResourceManager* resource = nullptr;
	class AudioSystem* audio = nullptr;
	class InputSystem* input = nullptr;
	class Renderer* renderer = nullptr;
};

struct GameSystem
{
	std::unique_ptr<GameContext> context = nullptr;
	std::unique_ptr<class Scene> scene = nullptr;
	std::unique_ptr<class ResourceManager>  resource = nullptr;
	std::unique_ptr<class AudioSystem> audio = nullptr;
	std::unique_ptr<class InputSystem> input = nullptr;
	std::unique_ptr<class Renderer> renderer = nullptr;

	GameSystem();
	~GameSystem();
};
