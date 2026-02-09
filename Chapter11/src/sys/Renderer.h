#pragma once
#include <vector>
#include <memory>
#include "../util/Math.h"
#include <SDL2/SDL.h>


struct GameConfig;
struct GameContext;

struct DirectionalLight
{
	Vector3 mDirection;
	Vector3 mDiffuseColor;
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(GameContext* context);
	~Renderer();

	bool Initialize(GameConfig& config);
	void Shutdown();
	void UnloadData();
	void Draw();

	Vector3 UnProject(const Vector3& screenPoint) const;

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddMeshComp(class MeshComponent* meshcomp);
	void RemoveMeshComp(class MeshComponent* mc);

	void SetLightUniforms(class Shader* shader);
	void SetViewMatrix(const Matrix4& view) noexcept { mView = view; }
	void SetAmbientLight(const Vector3& ambient) noexcept { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() noexcept { return mDirLight; }
	Matrix4& GetView() noexcept { return mView; }
	Vector2 GetScreenSize() const { return mScreenSize; }
	Vector2 GetCornerScreenSize() const { return mScreenSize * 0.5f; }
	void GetScreenDirection(Vector3& outstart, Vector3& outDir) const;
private:
	bool LoadShaders();
	void CreateSpriteVerts();

	SDL_Window* mWindow = nullptr;
	SDL_GLContext mGLContext;

	std::vector<class SpriteComponent*> mSprites;
	std::vector<class MeshComponent*> mMeshComps;

	std::unique_ptr<class VertexArray> mSpriteVerts;
	std::unique_ptr<class Shader> mSpriteShader;
	std::unique_ptr<class Shader> mMeshShader;
	
	Matrix4 mView;
	Matrix4 mProjection;
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	GameContext* mGameContext = nullptr;
	Vector2 mScreenSize;
};