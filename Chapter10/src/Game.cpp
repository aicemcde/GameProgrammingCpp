#include "Game.h"
#include "VertexArray.h"
#include "Shader.h"
#include <cstdint>
#include "Scene.h"
#include "Actor.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "Mesh.h"
#include "PlaneActor.h"
#include "SpriteComponent.h"
#include "AudioSystem.h"
#include "AudioComponent.h"
#include "MoveComponent.h"
#include "Log.h"
#include "InputSystem.h"
#include "FPSActor.h"
#include "Config.h"
#include "GameSystem.h"
#include "FollowActor.h"
#include "SplineActor.h"

Game::Game()
	:mIsRunning(true)
	, mTicksCount(0)
	, mCameraActor(nullptr)
{
	
}

Game::~Game()
{

}

bool Game::Initialize(GameConfig& config)
{
	Log::Info("start initialize");
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
	if (sdlResult != 0)
	{
		LOG_CRITICAL("SDL could not initialize!SDL_Error : {}", SDL_GetError());
		return false;
	}

	mGameSystem = std::make_unique<GameSystem>();
	

	if (!mGameSystem->renderer->Initialize(config))
	{
		LOG_CRITICAL("Renderer could not initialize!");
		return false;
	}

	if (!mGameSystem->audio->Initialize())
	{
		LOG_CRITICAL("Failed to initialize audio system");
		mGameSystem->audio->Shutdown();
		mGameSystem->audio = nullptr;
		return false;
	}

	
	mGameSystem->input->Initialize();
	mGameSystem->input->SetRelativeMouseMode(true);

	LoadData();

	mTicksCount = SDL_GetTicks();

	Log::Info("game initialize success");
	return true;
}

void Game::Shutdown()
{
	Log::Info("game shutdown");
	UnloadData();
	mGameSystem->input->Shutdown();
	mGameSystem->audio->Shutdown();
	SDL_Quit();
}

void Game::RunLoop()
{
	Log::Info("Start RunLoop");
	GLenum err;
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LOG_CRITICAL("GL_Error : {}", err);
		}
	}
}

void Game::ProcessInput()
{
	mGameSystem->input->PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		default:
			break;
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
			break;
		}
	}

	mGameSystem->input->Update();
	const InputState& state = mGameSystem->input->GetState();

	const uint8_t* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mGameSystem->scene->ProcessInput(state);

}

void Game::HandleKeyPress(int key)
{
	/*switch (key)
	{
	case '-':
	{
		float volume = mGameSystem->audio->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mGameSystem->audio->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		float volume = mGameSystem->audio->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mGameSystem->audio->SetBusVolume("bus:/", volume);
		break;
	}
	case 'e':
		mGameSystem->audio->PlayEvent("event:/Explosion2D");
		break;
	case 'm':
		mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
		break;
	case 'r':
		if (!mReverbSnap.IsValid())
		{
			mReverbSnap = mGameSystem->audio->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			mReverbSnap.Stop();
		}
		break;
	default:
		break;
	}*/
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	if (mFireSphere->GetPosition().x < 150.0f)
	{
		mSphereMoveComp->SetForwardSpeed(500.0f);
	}
	else if (mFireSphere->GetPosition().x > 850.0f)
	{
		mSphereMoveComp->SetForwardSpeed(-500.0f);
	}

	mGameSystem->scene->Update(deltaTime);
	mGameSystem->audio->SetListener(mGameSystem->renderer->GetView(), mCameraActor->GetPosition(), deltaTime);
	mGameSystem->audio->Update(deltaTime);
	//ColorfulBG(deltaTime);
}

void Game::GenerateOutput()
{
	mGameSystem->renderer->Draw();
}

void Game::LoadData()
{
	Log::Info("Start to loadData at Game class");
	//Actor
	Actor* a = mGameSystem->scene->CreateActor<Actor>();
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = a->AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameSystem->resource->GetMesh("Assets/Cube.gpmesh"));

	a = mGameSystem->scene->CreateActor<Actor>();
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = a->AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameSystem->resource->GetMesh("Assets/Sphere.gpmesh"));

	//Floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a = mGameSystem->scene->CreateActor<PlaneActor>();
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; ++i)
	{
		a = mGameSystem->scene->CreateActor<PlaneActor>();
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = mGameSystem->scene->CreateActor<PlaneActor>();
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	for (int i = 0; i < 10; ++i)
	{
		a = mGameSystem->scene->CreateActor<PlaneActor>();
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = mGameSystem->scene->CreateActor<PlaneActor>();
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	//Light
	mGameSystem->renderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mGameSystem->renderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.70f, -0.70f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	//Camera
	//mCameraActor = mScene->CreateActor<CameraActor>(this);
	mCameraActor = mGameSystem->scene->CreateActor<FollowActor>();

	//UI
	a = mGameSystem->scene->CreateActor<Actor>();
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = a->AddComponent_Pointer<SpriteComponent>();
	sc->SetTexture(mGameSystem->resource->GetTexture("Assets/HealthBar.png"));

	a = mGameSystem->scene->CreateActor<Actor>();
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = a->AddComponent_Pointer<SpriteComponent>();
	sc->SetTexture(mGameSystem->resource->GetTexture("Assets/Radar.png"));

	//spheres with audio
	a = mGameSystem->scene->CreateActor<Actor>();
	mFireSphere = a;
	a->SetPosition(Vector3(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	mc = a->AddComponent_Pointer<MeshComponent>();
	mc->SetMesh(mGameSystem->resource->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = a->AddComponent_Pointer<AudioComponent>(200.0f, Vector3(500.0f, -75.0f, 0.0f));
	ac->PlayEvent("event:/FireLoop", mGameSystem->context.get());
	mSphereMoveComp = a->AddComponent_Pointer<MoveComponent>();
	mSphereMoveComp->SetForwardSpeed(0.0f);

	//start music
	mMusicEvent = mGameSystem->audio->PlayEvent("event:/Music");
}

void Game::UnloadData()
{
	Log::Info("start to unload data");
	mGameSystem->renderer->UnloadData();
	mGameSystem->scene->Unload();
}

void Game::ColorfulBG(float deltaTime)
{
	if (mRGB_bgColor.z < 1.0f)
	{
		mRGB_bgColor.z += deltaTime / 5.0f;
		if (mRGB_bgColor.z > 1.0f)
		{
			mRGB_bgColor.z = 1.0f;
		}
	}
	glClearColor(mRGB_bgColor.x, mRGB_bgColor.y, mRGB_bgColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}