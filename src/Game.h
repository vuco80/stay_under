#ifndef SU_GAME_H
#define SU_GAME_H

#include <vuklib/Platform.h>
#include <vector>
#include <array>

//
// Enum, structs, constants
//
	
const int TOMB_ROWS = 3;
const int TOMB_COLS = 4;

enum Spawns {
	SPAWN_NONE = 0,
	SPAWN_SKELETON,
	SPAWN_ZOMBI,
	SPAWN_GHOST
};

class ITombsListener {
public:
	ITombsListener(){};
	~ITombsListener(){};
	
	virtual void onSpawnArrived(Spawns spawn) = 0;
};

class Tombs {
public:

	Tombs();
	~Tombs();
	
	void init(SDL_Texture* texSkeleton, SDL_Texture* texZombi,  SDL_Texture* texGhost, SDL_Texture* texTomb, ITombsListener* listener);
	void onUpdate();
	void render();
	bool checkHammerCollision(const SDL_Rect& hammer);
	int getScore() const;

private:
	
	struct Tomb {
		Spawns spawn;
		int spawnTimer;
		SDL_FRect collision;
	};
	
	int getMonsterSpeed(Spawns spawn);
	
	Tomb m_tombs[TOMB_ROWS][TOMB_COLS];
	Tomb m_arrived;
	SDL_Texture *m_texSkeleton, *m_texZombi, *m_texGhost, *m_texTomb;
	int m_spawnClock;
	ITombsListener* m_listener;
	int m_turn;
};

class Game : public ITombsListener, public vuk::IFrameTrackerListener {
public:
	Game();
	~Game();
	
	void init();	
	SDL_Texture* render();
	void onEvent(const SDL_Event* evt);
	void onUpdate();
	
	bool shouldClose() const;
	
	void quit();
	
	void onSpawnArrived(Spawns spawn) override;
	void onFrameTrackerTick(int frameTrackerId, int tickCount) override;
	
private:
	
	enum GameStates {
		MainMenuState,
		StartingState,
		ActionState,
		GameOverState
	};
	
	struct HammerState {
		bool up;
		int downTimer;
		SDL_FRect collision;
	};
	
	bool m_shouldClose;
	SDL_Texture *m_texBackground, *m_texTomb, *m_texSkeleton, *m_texHammerOn, *m_texHammerOff, *m_texTerrorBar;
	SDL_Texture *m_texTitle, *m_texCountdown, *m_texBlood, *m_texZombi, *m_texScore, *m_texGhost;
	SDL_Texture *m_canvas;
	Tombs m_tombs;
	SDL_FPoint m_mousePos;
	HammerState m_hammerState;
	int m_terror;
	GameStates m_state;
	int m_countdownIndex, m_countdownTimer;
	int m_bloodY;
	int m_scoreW, m_scoreH;
};

//
// GameInput
//

class GameInput : public vuk::IPlatformEventListener {
public:
	GameInput(Game* game);
	~GameInput();
	
	void OnEvent(const SDL_Event* evt) override;	
	bool shouldClose() const;
	
private:
	Game* m_game;
};

//
// GameFrame
//

class GameFrame : public vuk::IPlatformFrameListener {
public:
	GameFrame(Game* game);
	~GameFrame();
	
	void OnFrameUpdateStep() override;	
	void OnFrameRender(unsigned int elapsedMs) override;

private:
	Game* m_game;
};

#endif