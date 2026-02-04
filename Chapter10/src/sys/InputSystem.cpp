#include "InputSystem.h"
#include <SDL2/SDL_keyboard.h>
#include "../util/Log.h"
#include "../util/Math.h"
#include "GameSystem.h"
#include "../util/Config.h"

const int CONTROLLER_1D_DEADZONE = 250;
const int CONTROLLER_1D_MAXVALUE = 30000;

const float CONTROLLER_2D_DEADZONE = 8000.0f;
const float CONTROLLER_2D_MAXVALUE = 30000.0f;

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(int button) const
{
	Uint32 mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetKeyValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetKeyState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

InputSystem::InputSystem(GameContext* context)
	:mContext(context)
{

}

InputSystem::~InputSystem()
{

}

bool InputSystem::Initialize()
{
	Log::Info("InputSystem start to initialize");

	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);
	
	for (int i = 0; i < MAX_ACTIVE_PLAYER; ++i)
	{
		mControllerHandlers[i] = nullptr;
		memset(mState.Controller[i].mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
		memset(mState.Controller[i].mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	}

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			OnControllerConnected(i);
		}
	}

	InputActionConfig config;
	if (LoadConfig("config/keybinds.json", config))
	{
		mActionMap = config.actions;

		Log::Info("Key binds loaded");
	}
	else
	{
		Log::Error("Failed to load key binds.");
	}

	

	Log::Info("InputSystem complete to initialize");
	return true;
}

void InputSystem::Shutdown()
{
	for (int i = 0; i < MAX_ACTIVE_PLAYER; ++i)
	{
		if (mControllerHandlers[i])
		{
			SDL_GameControllerClose(mControllerHandlers[i]);
			mControllerHandlers[i] = nullptr;
		}
	}
	mJoystickID_To_ControllerPlayerID_map.clear();
}

void InputSystem::PrepareForUpdate()
{
	memcpy(
		mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES
	);
	for (int i = 0; i < MAX_ACTIVE_PLAYER; ++i)
	{
		if (mState.Controller[i].mIsConnected)
		{
			memcpy(mState.Controller[i].mPrevButtons, mState.Controller[i].mCurrButtons, SDL_CONTROLLER_BUTTON_MAX);
		}
	}

	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;

	mState.Mouse.mScrollWheel = Vector2::Zero;
}

void InputSystem::Update()
{
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
	}
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	for (int i = 0; i < MAX_ACTIVE_PLAYER; ++i)
	{
		if (mState.Controller[i].mIsConnected)
		{
			SDL_GameController* handle = mControllerHandlers[i];
			ControllerState& state = mState.Controller[i];
			
			if (!handle)
			{
				LOG_WARN("mControllerHandler is not registered: {}", i);
				continue;
			}

			for (int btn = 0; btn < SDL_CONTROLLER_BUTTON_MAX; ++btn)
			{
				state.mCurrButtons[btn] = SDL_GameControllerGetButton(handle, SDL_GameControllerButton(btn));
			}

			state.mLeftTrigger = Filter1D(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
			state.mRightTrigger = Filter1D(SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

			x = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTX);
			y = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_LEFTY);
			state.mLeftStick = Filter2D(x, y);

			x = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTX);
			y = SDL_GameControllerGetAxis(handle, SDL_CONTROLLER_AXIS_RIGHTY);
			state.mRightStick = Filter2D(x, y);
		}
	}
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2
		(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y)
		);
		break;

	case SDL_CONTROLLERDEVICEADDED:
		OnControllerConnected(event.cdevice.which);
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		OnControllerDisconnected(event.cdevice.which);
		break;
	default:
		break;
	}
}

const ControllerState& InputSystem::GetControllerState(int playerIndex) const
{
	if (playerIndex < 0 || playerIndex >= MAX_ACTIVE_PLAYER)
	{
		Log::Warn("GetControllerState : playerIndex is too large");
		return mState.Controller[0];
	}
	return mState.Controller[playerIndex];
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);
	mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input)
{
	float retVal = 0.0f;

	int absValue = input > 0 ? input : -input;

	if (absValue > CONTROLLER_1D_DEADZONE)
	{
		retVal = static_cast<float>(absValue - CONTROLLER_1D_DEADZONE) / (CONTROLLER_1D_MAXVALUE - CONTROLLER_1D_DEADZONE);
		retVal = input > 0 ? retVal : retVal * -1.0f;
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}
	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	if (length < CONTROLLER_2D_DEADZONE)
	{
		dir = Vector2::Zero;
	}
	else
	{
		float f = (length - CONTROLLER_2D_DEADZONE) / (CONTROLLER_2D_MAXVALUE / CONTROLLER_2D_DEADZONE);
		f = Math::Clamp(f, 0.0f, 1.0f);
		dir *= f / length;
	}
	return dir;
}

int InputSystem::FindFreePlayerSlot() const
{
	for (int i = 0; i < MAX_ACTIVE_PLAYER; ++i)
	{
		if (mControllerHandlers[i] == nullptr)
		{
			return i;
		}
	}
	return -1;
}

void InputSystem::OnControllerConnected(int deviceIndex)
{
	int playerIndex = FindFreePlayerSlot();
	if (playerIndex == -1)
	{
		LOG_WARN("Max players reached, ignoring controller index: {}", deviceIndex);
		return;
	}

	SDL_GameController* controller = SDL_GameControllerOpen(deviceIndex);
	if (!controller)
	{
		LOG_WARN("Failed to open controller: {}", SDL_GetError());
		return;
	}

	SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
	SDL_JoystickID joystickID = SDL_JoystickInstanceID(joystick);
	
	mControllerHandlers[playerIndex] = controller;
	mJoystickID_To_ControllerPlayerID_map[joystickID] = playerIndex;

	mState.Controller[playerIndex].mIsConnected = true;

	LOG_INFO("Controller connected: Player {}", playerIndex);
}

void InputSystem::OnControllerDisconnected(SDL_JoystickID id)
{
	auto iter = mJoystickID_To_ControllerPlayerID_map.find(id);
	if (iter != mJoystickID_To_ControllerPlayerID_map.end())
	{
		int playerIndex = iter->second;

		if (mControllerHandlers[playerIndex])
		{
			SDL_GameControllerClose(mControllerHandlers[playerIndex]);
			mControllerHandlers[playerIndex] = nullptr;
		}

		mState.Controller[playerIndex].mIsConnected = false;

		mJoystickID_To_ControllerPlayerID_map.erase(iter);

		LOG_INFO("Controller disconnected: Player {}", playerIndex);
	}
}

ButtonState InputSystem::GetMappedButtonState(const std::string& actionName)
{
	auto iter = mActionMap.find(actionName);
	if (iter == mActionMap.end())
	{
		LOG_WARN("Action is not found: {}", actionName);
		return ENone;
	}

	for (const auto& action : iter->second)
	{
		switch (action.deviceType)
		{
		case InputDeviceType::Keyboard:
		{
			SDL_Scancode key = static_cast<SDL_Scancode>(action.code);
			return mState.Keyboard.GetKeyState(key);
		}
		case InputDeviceType::Mouse:
		{
			int button = action.code;
			return mState.Mouse.GetButtonState(button);
		}
		case InputDeviceType::Controller:
		{
			SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(action.code);
			int idx = action.playerIndex;

			if (0 < idx && idx  < MAX_ACTIVE_PLAYER)
			{
				return mState.Controller[idx].GetKeyState(button);
			}

			return ENone;
			break;
		}
		default:
			return ENone;
			break;
		}
	}
}