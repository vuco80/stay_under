#include "Game.h"

const int FRAME_DURATION_NEW_SPAWN = 80;
const int FRAME_DURATION_SPAWN_ADVANCE = 65;
const int FRAME_DURATION_ARRIVED = 30;

Tombs::Tombs() {
}

Tombs::~Tombs() {
}

void Tombs::init(SDL_Texture* texSkeleton, SDL_Texture* texZombi, SDL_Texture* texGhost, SDL_Texture* texTomb, ITombsListener* listener) {
	for(int r=0; r<TOMB_ROWS; ++r) {
		for(int c=0; c<TOMB_COLS; ++c) {
			m_tombs[r][c].spawn = SPAWN_NONE;
			m_tombs[r][c].spawnTimer = 0;
			m_tombs[r][c].collision.y = 50 + (r*116) + 65;
			m_tombs[r][c].collision.x = 50 + (c*175) + 50;
			m_tombs[r][c].collision.w = 80;
			m_tombs[r][c].collision.h = 50;
			
		}
	}
	
	m_arrived.spawn = SPAWN_NONE;
	
	m_spawnClock = 0;
	
	m_texSkeleton = texSkeleton;
	m_texTomb = texTomb;
	m_texZombi = texZombi;
	m_texGhost = texGhost;
	m_listener = listener;
	m_turn = 0;
}

void Tombs::onUpdate() {
	// new spawns
	
	m_spawnClock++;
	
	if(m_spawnClock >= FRAME_DURATION_NEW_SPAWN) {
		if(m_turn < 20) {
			int rand_col = rand() % TOMB_COLS;
			if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
			m_tombs[0][rand_col].spawn = SPAWN_SKELETON;
			m_tombs[0][rand_col].spawnTimer = 0;
			m_spawnClock = 0;
		} else if(m_turn < 40) {
			if(rand() % 2 == 0) {
				int rand_col = rand() % TOMB_COLS;
				if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
				m_tombs[0][rand_col].spawn = SPAWN_SKELETON;
				m_tombs[0][rand_col].spawnTimer = 0;
			} else {
				int rand_col = rand() % TOMB_COLS;
				if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
				m_tombs[0][rand_col].spawn = SPAWN_ZOMBI;
				m_tombs[0][rand_col].spawnTimer = 0;
			}
			m_spawnClock = 0;
		} else {
			int type = rand() % 5;
			if(type == 0) {
				int rand_col = rand() % TOMB_COLS;
				if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
				m_tombs[0][rand_col].spawn = SPAWN_SKELETON;
				m_tombs[0][rand_col].spawnTimer = 0;
			} else if(type == 1 || type == 2) {
				int rand_col = rand() % TOMB_COLS;
				if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
				m_tombs[0][rand_col].spawn = SPAWN_ZOMBI;
				m_tombs[0][rand_col].spawnTimer = 0;
			} else {
				int rand_col = rand() % TOMB_COLS;
				if(m_tombs[0][rand_col].spawn != SPAWN_NONE) rand_col = (rand_col + 1) % TOMB_COLS;
				m_tombs[0][rand_col].spawn = SPAWN_GHOST;
				m_tombs[0][rand_col].spawnTimer = 0;
			}
			m_spawnClock = 0;
		}
		
		m_turn++;
	}
	
	// spawn timer
	
	for(int r=0; r<TOMB_ROWS; ++r) {
		for(int c=0; c<TOMB_COLS; ++c) {
			if(m_tombs[r][c].spawn != SPAWN_NONE){
				m_tombs[r][c].spawnTimer++;
				if(m_tombs[r][c].spawnTimer >= FRAME_DURATION_SPAWN_ADVANCE - (m_turn / 5) - getMonsterSpeed(m_tombs[r][c].spawn)) {
					m_tombs[r][c].spawnTimer = 0;
					if(r < TOMB_ROWS - 1) {
						int rand_col = rand() % TOMB_COLS;
						if(m_tombs[r+1][rand_col].spawn != SPAWN_NONE) {
							rand_col = (rand_col + 1) % TOMB_COLS;
						}
						Spawns s = m_tombs[r][c].spawn;
						m_tombs[r][c].spawn = SPAWN_NONE;
						m_tombs[r+1][rand_col].spawn = s;
						m_tombs[r][c].spawnTimer = 0;
						m_tombs[r+1][rand_col].spawnTimer = 0;
					}
					else {
						// spawn has arrived!
						m_arrived.spawn = m_tombs[r][c].spawn;
						m_arrived.spawnTimer = 0;
						
						m_tombs[r][c].spawn = SPAWN_NONE;
						m_tombs[r][c].spawnTimer = 0;
						
						m_listener->onSpawnArrived(m_arrived.spawn);
					}
				}
			}
		}
	}
	
	// arrived timer 
	if(m_arrived.spawn != SPAWN_NONE) {
		m_arrived.spawnTimer++;
		if(m_arrived.spawnTimer >= FRAME_DURATION_ARRIVED) {
			m_arrived.spawnTimer = 0;
			m_arrived.spawn = SPAWN_NONE;
		}
	}
}

void Tombs::render() {
	SDL_FRect dst;
	
	dst.w = 175;
	dst.h = 116;
	for(int r=0; r<3; ++r) {
		for(int c=0; c<4; ++c) {
			dst.x = 50 + c * 175;
			dst.y = 50 + r * 116;
			vuk::Platform::instance()->renderTexture(m_texTomb, nullptr, &dst);
		}
	}
	
	// spawns
	for(int r=0; r<TOMB_ROWS; ++r) {
		for(int c=0; c<TOMB_COLS; ++c) {
			dst.x = 50 + c * 175;
			dst.y = 50 + r * 116;
			dst.w = 175;
			dst.h = 116;
			switch(m_tombs[r][c].spawn) {
				case SPAWN_NONE:
				break;
				
				case SPAWN_SKELETON:
				vuk::Platform::instance()->renderTexture(m_texSkeleton, nullptr, &dst);
				break;
				
				case SPAWN_ZOMBI:
				vuk::Platform::instance()->renderTexture(m_texZombi, nullptr, &dst);
				break;
				
				case SPAWN_GHOST:
				vuk::Platform::instance()->renderTexture(m_texGhost, nullptr, &dst);
				break;
			}
			
			//vuk::Platform::instance()->renderRect(true, {255,0,0,255}, m_tombs[r][c].collision);
		}
	}
	
	// arrived 
	
	if(m_arrived.spawn != SPAWN_NONE) {
		dst.x = 0;
		dst.y = 30;
		dst.w = 700;
		dst.h = 464;
		switch(m_arrived.spawn) {
			case SPAWN_NONE:
			break;
			
			case SPAWN_SKELETON:
			vuk::Platform::instance()->renderTexture(m_texSkeleton, nullptr, &dst);
			break;
			
			case SPAWN_ZOMBI:
			vuk::Platform::instance()->renderTexture(m_texZombi, nullptr, &dst);
			break;
			
			case SPAWN_GHOST:
			vuk::Platform::instance()->renderTexture(m_texGhost, nullptr, &dst);
			break;
		}
	}
}

bool Tombs::checkHammerCollision(const SDL_Rect& hammer) {
	for(int r=0; r<TOMB_ROWS; ++r) {
		for(int c=0; c<TOMB_COLS; ++c) {
			if(m_tombs[r][c].spawn == SPAWN_NONE) continue;
			SDL_Rect B = {(int)m_tombs[r][c].collision.x, 
				(int)m_tombs[r][c].collision.y, 
				(int)m_tombs[r][c].collision.w, 
				(int)m_tombs[r][c].collision.h};
			if(SDL_HasRectIntersection(&hammer, &B)) {
				// HIT!
				m_tombs[r][c].spawn = SPAWN_NONE;
				m_tombs[r][c].spawnTimer = 0;
				// accelerate next spawn
				m_spawnClock += FRAME_DURATION_NEW_SPAWN / 2;
				return true;
			}
		}
	}
	return false;
}

int Tombs::getMonsterSpeed(Spawns spawn) {
	switch(spawn) {
		case SPAWN_NONE:
		return 0;
		
		case SPAWN_SKELETON:
		return 0;
		
		case SPAWN_ZOMBI:
		return 10;
		
		case SPAWN_GHOST:
		return 15;
		
		default:
		return 0;
	}
}

int Tombs::getScore() const {
	return m_turn;
}
