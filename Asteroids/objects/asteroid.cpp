#include "objects/asteroid.h"

Asteroid::Asteroid() {
	glm::mat4 asteroid(1.0f);
	angle = 0.0f;
	size = 0.0f;
	hits = 0;
	spawn = 0;
	lives = 0;
	speed = 0;
	active = true;
}

glm::mat4 Asteroid::getAsteroid() { return asteroid; }

float Asteroid::getAngle() { return angle; }
float Asteroid::getSize() { return size; }
float Asteroid::getPosX() { return asteroid[3][0]; }
float Asteroid::getPosY() { return asteroid[3][1]; }
float Asteroid::getSpeed() { return speed; }
bool Asteroid::getActive() { return active; }
int Asteroid::getHits() { return hits; }
int Asteroid::getLives() { return lives; }
int Asteroid::getSpawn() { return spawn; }
float Asteroid::getSin() { return asteroid[0][1]; }
float Asteroid::getCos() { return asteroid[0][0]; }

void Asteroid::setAsteroid(glm::mat4 obj) { asteroid = obj; }
void Asteroid::setAngle(float ang) { angle = ang; }
void Asteroid::setSize(float s) { size = s; }
void Asteroid::setActive(bool val) { active = val; }
void Asteroid::setHits() { hits++; }
void Asteroid::setLives(int val) { lives = val; }
void Asteroid::setSpawn(int val) { spawn = val; }
void Asteroid::setSpeed(float val) { speed = val; }
