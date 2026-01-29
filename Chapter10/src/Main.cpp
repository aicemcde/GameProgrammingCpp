#include "Log.h"
#include "Game.h"
#include "Config.h"

int main(int argc, char** argv)
{
	Log::Init();

	GameConfig config;
	if (!LoadConfig("config/config.json", config))
	{
		LOG_ERROR("Failed to load config");
		return 0;
	}

	Game game;
	bool success = game.Initialize(config);
	if (success)
	{
		game.RunLoop();
	}
	else
	{
		LOG_ERROR("Failed to initialize");
	}
	game.Shutdown();
	return 0;
}