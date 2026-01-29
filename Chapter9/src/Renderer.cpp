#include "Renderer.h"
#include "SpriteComponent.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "MeshComponent.h"
#include <SDL_ttf.h>
#include "Math.h"
#include "Config.h"
#include "GameSystem.h"


Renderer::Renderer(GameContext* context)
	:mGameContext(context)
	, mGLContext(nullptr)
{
	
}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(GameConfig& config)
{
	LOG_INFO("Renderer start to initialize");
	//ランタイムチェックはCreateWindowでできるので、SetAttributeでする必要はない
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	LOG_INFO("Width: {}, Height: {}", config.window.width, config.window.height);
	if (config.window.fullScreen)
	{
		mWindow = SDL_CreateWindow("Asteroids_OpenGL", 100, 100, config.window.width, config.window.height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
		LOG_INFO("Window is full screen");
	}
	else
	{
		mWindow = SDL_CreateWindow("Asteroids_OpenGL", 100, 100, config.window.width, config.window.height, SDL_WINDOW_OPENGL);
		LOG_INFO("window");
	}

	mScreenSize = Vector2(config.window.width, config.window.height);
	
	
	if (!mWindow)
	{
		SDL_Log("SDL cannot create Window! SDL_Error : %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() != 0)
	{
		SDL_Log("TTF could not initialize!");
		return false;
	}

	mGLContext = SDL_GL_CreateContext(mWindow);
	if (!mGLContext)
	{
		SDL_Log("Failed to create context : %s", SDL_GetError());
		return false;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("GLEW could not initialize!");
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
	}

	glGetError();

	if (!LoadShaders())
	{
		SDL_Log("Failed to load shader");
		return false;
	} 

	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	mSpriteShader->Unload();
	mMeshShader->Unload();
	UnloadData();
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	mSprites.clear();
	mMeshComps.clear();
}

void Renderer::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	mMeshShader->SetActive();
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	SetLightUniforms(mMeshShader.get());

	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader.get());
	}


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader.get());
	}

	SDL_GL_SwapWindow(mWindow);
}

Vector3 Renderer::UnProject(const Vector3& screenPoint) const
{
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= mScreenSize.x * 0.5f;
	deviceCoord.y /= mScreenSize.y * 0.5f;
	
	Matrix4 unProjection = mView * mProjection;
	unProjection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unProjection);
}

void Renderer::AddSprite(SpriteComponent* sc)
{
	int myOrder = sc->GetUpdateOrder();
	auto iter = std::ranges::lower_bound(mSprites, myOrder, {}, &SpriteComponent::GetUpdateOrder);
	mSprites.insert(iter, sc);
}

void Renderer::RemoveSprite(SpriteComponent* sc)
{
	auto iter = std::ranges::find(mSprites, sc);
	if (iter != mSprites.end())
	{
		mSprites.erase(iter);
	}
}

void Renderer::AddMeshComp(MeshComponent* mc)
{
	mMeshComps.emplace_back(mc);
}

void Renderer::RemoveMeshComp(MeshComponent* mc)
{
	auto iter = std::ranges::find(mMeshComps, mc);
	if (iter != mMeshComps.end())
	{
		std::iter_swap(iter, mMeshComps.end() - 1);
		mMeshComps.pop_back();
	}
}

void Renderer::CreateSpriteVerts()
{ 
	float vertexBuffer[] = {
	-0.5f, 0.5f, 0.f,0.0f, 0.f, 0.f, 0.f, 0.f,
	0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
	0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,
	-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f
	};

	unsigned int indexBuffer[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = std::make_unique<VertexArray>(vertexBuffer, 4, indexBuffer, 6);
}

bool Renderer::LoadShaders()
{ 
	mSpriteShader = std::make_unique<Shader>();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1024.f, 768.f);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	mMeshShader = std::make_unique<Shader>();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	mMeshShader->SetActive();
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mScreenSize.x, mScreenSize.y, 25.0f, 10000.0f);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	return true;
}

void Renderer::SetLightUniforms(Shader* shader)
{
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = UnProject(outStart);

	screenPoint.z = 0.9f;
	Vector3 end = UnProject(screenPoint);

	outDir = end - outStart;
	outDir.Normalize();
}