#pragma once
#include "../util/Math.h"
#include <SDL2/SDL.h>
#include <gl/glew.h>
#include <memory>
#include <vector>
#include "../util/SoundEvent.h"

struct GameConfig;
struct GameSystem;

class Game
{
public:
	Game();
	~Game();
	bool Initialize(GameConfig& config);
	void RunLoop();
	void Shutdown();
	void SetGameRunning(bool running) { mIsRunning = running; }

	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
	void AddPlanes(class PlaneActor*);
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void ColorfulBG(float deltaTime);


	static Game* sInstance;

	bool mIsRunning = false;

	std::unique_ptr<GameSystem> mGameSystem;
	
	class Actor* mCameraActor;

	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	Uint32 mTicksCount;

	Vector3 mRGB_bgColor;

	class Actor* mFireSphere;
	class MoveComponent* mSphereMoveComp;
	std::vector<class PlaneActor*> mPlanes;
};
