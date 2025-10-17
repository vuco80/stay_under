#include "Game.h"
#include <sstream>

//
// Game
//

const int FRAME_DURATION_HAMMER_OFF = 15;
const int MAX_TERROR = 596;
const int FRAME_DURATION_COUNTDOWN = 60;
const int FT_GAMEOVER_BLOOD = 1;
const int FREQUENCE_GAMEOVER_BLOOD = 1;

Game::Game(){}
Game::~Game(){
	vuk::Platform::instance()->destroyTexture(&m_texBackground);
	vuk::Platform::instance()->destroyTexture(&m_texTomb);
	vuk::Platform::instance()->destroyTexture(&m_texSkeleton);
	vuk::Platform::instance()->destroyTexture(&m_texHammerOn);
	vuk::Platform::instance()->destroyTexture(&m_texHammerOff);
	vuk::Platform::instance()->destroyTexture(&m_texTerrorBar);
	vuk::Platform::instance()->destroyTexture(&m_texTitle);
	vuk::Platform::instance()->destroyTexture(&m_texCountdown);
	vuk::Platform::instance()->destroyTexture(&m_texBlood);
	vuk::Platform::instance()->destroyTexture(&m_texZombi);
	vuk::Platform::instance()->destroyTexture(&m_texScore);
	vuk::Platform::instance()->destroyTexture(&m_texGhost);
	vuk::Platform::instance()->destroyTexture(&m_canvas);
}

void Game::init() {
	int tw, th;
	int result = vuk::Platform::instance()->loadTexture("assets/textures/background.png", &m_texBackground, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texBackground %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/tomb.png", &m_texTomb, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texTomb %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/skeleton.png", &m_texSkeleton, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texSkeleton %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/hammer_on.png", &m_texHammerOn, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texHammerOn %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/hammer_off.png", &m_texHammerOff, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texHammerOff %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/terrorbar.png", &m_texTerrorBar, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texTerrorBar %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/title.png", &m_texTitle, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texTitle %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/countdown.png", &m_texCountdown, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texCountdown %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/blood.png", &m_texBlood, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texBlood %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/zombi.png", &m_texZombi, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texZombi %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadTexture("assets/textures/ghost.png", &m_texGhost, &tw, &th);
	if(result != 0) {
		SDL_Log("error loading graphic for m_texGhost %d", result);
		return;
	}
	result = vuk::Platform::instance()->createTexture(&m_canvas, 800, 450);
	if(result != 0) {
		SDL_Log("error creating texture for canvas %d", result);
		return;
	}
	result = vuk::Platform::instance()->loadFont("assets/fonts/QueenEmpress.ttf", 32, 1);
	if(result != 0) {
		SDL_Log("error creating font for QueenEmpress");
		return;
	}
	
	m_hammerState.up = true;
	m_hammerState.downTimer = 0;
	m_hammerState.collision.x = 30;
	m_hammerState.collision.y = 78;
	m_hammerState.collision.w = 108;
	m_hammerState.collision.h = 35;
	
	m_tombs.init(m_texSkeleton, m_texZombi, m_texGhost, m_texTomb, this);
	
	m_terror = 0;
	m_state = MainMenuState;
	m_shouldClose = false;
	m_countdownIndex = 0;
	m_countdownTimer = 0;
}

void Game::onUpdate() {
	
	switch(m_state) {
		
		case StartingState:
		{
			if(m_countdownIndex < 3) {
				m_countdownTimer++;
				if(m_countdownTimer >= FRAME_DURATION_COUNTDOWN) {
					m_countdownTimer = 0;
					m_countdownIndex++;
					SDL_Log("countdown %d", m_countdownIndex);
				}
			}
			
			if(m_countdownIndex == 3) {
				m_state = ActionState;
				m_countdownIndex = 0;
				m_countdownTimer = 0;
			}
			
		} break;
		
		case ActionState:
		{
			if(m_terror == MAX_TERROR) {
				m_state = GameOverState;
				vuk::Platform::instance()->clearFrameTrackers();
				vuk::Platform::instance()->addFrameTracker(FT_GAMEOVER_BLOOD, FREQUENCE_GAMEOVER_BLOOD, this);
				std::stringstream ss;
				ss << m_tombs.getScore() << " Punti";
				vuk::Platform::instance()->createTextTexture(1, ss.str(), {255,255,255,255}, 800, &m_texScore, &m_scoreW, &m_scoreH);
				return;
			}
			
			// hammer
			
			if(m_hammerState.up == false) {
				m_hammerState.downTimer++;
				if(m_hammerState.downTimer >= FRAME_DURATION_HAMMER_OFF) {
					m_hammerState.up = true;
					m_hammerState.downTimer = 0;
				}
			}
			
			m_tombs.onUpdate();
		} break;
		
		case GameOverState:
		{
			
		} break;
		
		default:
		break;
	}
}

SDL_Texture* Game::render() {
	
	vuk::Platform::instance()->setRenderTarget(m_canvas);
	vuk::Platform::instance()->renderClear();
	vuk::Platform::instance()->renderTexture(m_texBackground, nullptr, nullptr);
	
	SDL_FRect dst;
	
	switch(m_state) {
		
		case MainMenuState:
		{
			vuk::Platform::instance()->renderTexture(m_texTitle, nullptr, nullptr);
		} break;
		
		case StartingState:
		{
			m_tombs.render();
			
			SDL_FRect src = {(float)(70 + (230 * m_countdownIndex)), 0, 230, 450};
			dst.x = 285;
			dst.y = 0;
			dst.w = 230;
			dst.h = 450;
			vuk::Platform::instance()->renderTexture(m_texCountdown, &src, &dst);
		} break;
		
		case ActionState:
		{
			m_tombs.render();
			
			// hammer 
			dst.x = m_mousePos.x - (175/2);
			dst.y = m_mousePos.y - (116/2);
			dst.w = 175;
			dst.h = 116;
			if(m_hammerState.up)
				vuk::Platform::instance()->renderTexture(m_texHammerOn, nullptr, &dst);
			else vuk::Platform::instance()->renderTexture(m_texHammerOff, nullptr, &dst);
			
			// terror
			dst.x = 100;
			dst.y = 20;
			dst.w = 600;
			dst.h = 30;
			vuk::Platform::instance()->renderRect(true, {0,0,0,255}, dst);
			
			dst.x = 102;
			dst.y = 22;
			dst.w = m_terror;
			dst.h = 26;
			vuk::Platform::instance()->renderRect(true, {255,0,0,255}, dst);
			
			dst.x = 100;
			dst.y = 20;
			dst.w = 600;
			dst.h = 30;
			vuk::Platform::instance()->renderTexture(m_texTerrorBar, nullptr, &dst);
		} break;
		
		case GameOverState:
		{
			m_tombs.render();
			
			// terror
			dst.x = 100;
			dst.y = 20;
			dst.w = 600;
			dst.h = 30;
			vuk::Platform::instance()->renderRect(true, {0,0,0,255}, dst);
			
			dst.x = 102;
			dst.y = 22;
			dst.w = m_terror;
			dst.h = 26;
			vuk::Platform::instance()->renderRect(true, {255,0,0,255}, dst);
			
			dst.x = 100;
			dst.y = 20;
			dst.w = 600;
			dst.h = 30;
			vuk::Platform::instance()->renderTexture(m_texTerrorBar, nullptr, &dst);
			
			dst.x = 0;
			dst.y = -900 + m_bloodY;
			dst.w = 800;
			dst.h = 900;
			vuk::Platform::instance()->renderTexture(m_texBlood, nullptr, &dst);
			
			dst.x = 400 - m_scoreW / 2;
			dst.y = 225 - m_scoreH / 2;
			dst.w = m_scoreW;
			dst.h = m_scoreH;
			vuk::Platform::instance()->renderTexture(m_texScore, nullptr, &dst);
		} break;
		
		default:
		break;
	}
	
	
	vuk::Platform::instance()->setRenderTarget(nullptr);
	
	return m_canvas;
}

void Game::onEvent(const SDL_Event* evt) {
	switch(evt->type) {
		case SDL_EVENT_MOUSE_MOTION:
		if(m_state == ActionState) {
			m_mousePos.x = evt->motion.x;
			m_mousePos.y = evt->motion.y;
		}
		break;
		
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if(m_state == MainMenuState) {
			SDL_Point mouse = {(int)evt->button.x, (int)evt->button.y};
			SDL_Rect quitRect = {385, 328, 73, 40};
			SDL_Rect playRect = {547,337,91,43};
			if(SDL_PointInRect(&mouse, &quitRect)) {
				quit();
			}
			else if(SDL_PointInRect(&mouse, &playRect)) {
				m_state = StartingState;
			}
		}
		else if(m_state == ActionState) {
			if(m_terror < MAX_TERROR) {
				if(m_hammerState.up) m_hammerState.up = false;
				SDL_Rect A = {(int)(m_hammerState.collision.x + m_mousePos.x - (175/2)), 
					(int)(m_hammerState.collision.y + m_mousePos.y - (116/2)), 
					(int)m_hammerState.collision.w, 
					(int)m_hammerState.collision.h};
				if(m_tombs.checkHammerCollision(A)) {
					// terror decreases
					if(m_terror > 0) m_terror -= 10;
				}
			}
		}
		break;
	}
}

void Game::onSpawnArrived(Spawns spawn) {
	switch(spawn) {
		case SPAWN_SKELETON:
		m_terror += 50;
		break;
		
		case SPAWN_ZOMBI:
		m_terror += 60;
		break;
		
		case SPAWN_GHOST:
		m_terror += 70;
		break;
		
		default:
		m_terror += 50;
		break;
	}
	if(m_terror >= MAX_TERROR) m_terror = MAX_TERROR;
}

void Game::onFrameTrackerTick(int frameTrackerId, int tickCount) {
	switch(frameTrackerId) {
		case FT_GAMEOVER_BLOOD:
		m_bloodY = tickCount;
		if(m_bloodY >= 900) {
			vuk::Platform::instance()->removeFrameTracker(frameTrackerId);
			
			m_tombs.init(m_texSkeleton, m_texZombi, m_texGhost, m_texTomb, this);
			m_terror = 0;
			m_state = MainMenuState;
			vuk::Platform::instance()->destroyTexture(&m_texScore);
			m_texScore = nullptr;			
		}
		break;
	}
}

bool Game::shouldClose() const {
	return m_shouldClose;
}

void Game::quit() {
	m_shouldClose = true;
}

//
// GameInput
//

GameInput::GameInput(Game* game) : m_game(game) {}
GameInput::~GameInput(){}

void GameInput::OnEvent(const SDL_Event* evt) {
	switch(evt->type) {
		case SDL_EVENT_QUIT:
		m_game->quit();
		break;
	}
	m_game->onEvent(evt);
}

bool GameInput::shouldClose() const { return m_game->shouldClose(); }

//
// GameFrame
//

GameFrame::GameFrame(Game* game) : m_game(game) {}
GameFrame::~GameFrame(){}

void GameFrame::OnFrameUpdateStep() {
	m_game->onUpdate();
}

void GameFrame::OnFrameRender(unsigned int elapsedMs) {
	if(elapsedMs > 20) SDL_Log("[WARNING] frame slow %d ms (%f fps)", elapsedMs, 1000.0f / elapsedMs);
	
	vuk::Platform::instance()->renderClear();
	SDL_Texture* scene = m_game->render();
	
	if(scene != nullptr) {
		vuk::Platform::instance()->renderTexture(scene, nullptr, nullptr);
	}
}
