#pragma once
#include <string>
#include <json.hpp>
#include "Log.h"
#include <fstream>
#include <iostream>

struct WindowConfig
{
	std::string title = "Game";
	int width = 1024;
	int height = 768;
	bool fullScreen = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(WindowConfig, title, width, height, fullScreen)
};

struct AudioConfig
{
	float master_volume;
	bool mute;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AudioConfig, master_volume, mute)
};

struct GameConfig
{
	WindowConfig window;
	AudioConfig audio;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameConfig, window, audio)
};

enum class InputDeviceType
{
	Keyboard,
	Mouse,
	Controller
};

struct InputActions
{
	InputDeviceType deviceType;
	int code;
	int playerIndex;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(InputActions, deviceType, code, playerIndex)
};

struct InputActionConfig
{
	std::unordered_map<std::string, std::vector<InputActions>> actions;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(InputActionConfig, actions)
};

template<typename T>
static bool LoadConfig(const std::string& path, T& outConfig)
{
	LOG_INFO("Load config: {}", path);

	/*
	if (!fs::exists(path))
	{
		LOG_WARN("Config file not found: {}. Creating default.", path);
		SaveJson(path, outConfig);
		return true;
	}
	*/

	std::ifstream file(path);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open config file: {}", path);
		return false;
	}

	try
	{
		nlohmann::json j;
		file >> j;
		outConfig = j.get<T>();

		LOG_INFO("Config loaded successfully");
		return true;
	}
	catch (const nlohmann::json::exception& e)
	{
		LOG_CRITICAL("JSON parsing Error: {} path: {}", e.what(), path);
		return false;
	}
}

template<typename T>
static bool SaveJson(const std::string& path, const T& inConfig)
{
	std::ofstream file(path);
	if (!file.is_open())
	{
		LOG_ERROR("Failed to open file for writing: {}", path);
		return false;
	}

	try
	{
		nlohmann::json j = inConfig;
		file << j.dump(4);
		LOG_INFO("Saved file: {}", path);
		return true;
	}
	catch (const nlohmann::json::exception& e)
	{
		LOG_ERROR("Json write Error: {}", e.what());
		return false;
	}
}