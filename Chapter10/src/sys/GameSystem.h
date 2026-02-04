#pragma once
#include <memory>

class Scene;
class ResourceManager;
class AudioSystem;
class InputSystem;
class Renderer;
class PhysWorld;

struct GameContext
{
	class Scene* scene = nullptr;
	class ResourceManager* resource = nullptr;
	class AudioSystem* audio = nullptr;
	class InputSystem* input = nullptr;
	class Renderer* renderer = nullptr;
	class PhysWorld* physWorld = nullptr;
	class Game* game = nullptr;
};

struct GameSystem
{
	std::unique_ptr<GameContext> context = nullptr;
	std::unique_ptr<class Scene> scene = nullptr;
	std::unique_ptr<class ResourceManager>  resource = nullptr;
	std::unique_ptr<class AudioSystem> audio = nullptr;
	std::unique_ptr<class InputSystem> input = nullptr;
	std::unique_ptr<class Renderer> renderer = nullptr;
	std::unique_ptr<PhysWorld> physWorld = nullptr;

	GameSystem(class Game* game);
	~GameSystem();
};
