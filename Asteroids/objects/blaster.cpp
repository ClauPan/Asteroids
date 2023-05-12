#include "blaster.h"

Blaster::Blaster() {
	glm::mat4 blaster(1.0f);
	angle = 0.0f;
	active = false;
	veloX = 0.0f;
	veloY = 0.0f;
}

float Blaster::calculate_angle(glm::mat4 obj) {
	float ac = (acos(obj[0][0])) * 180.0 / PI;
	float as = (asin(obj[0][1])) * 180.0 / PI;
	if (as < 0) { return -ac; }
	else { return ac; }
}

glm::mat4 Blaster::getBlaster() { return blaster; }

float Blaster::getAngle() { return angle; }
bool Blaster::getActive() { return active; }
float Blaster::getPosX() { return blaster[3][0]; }
float Blaster::getPosY() { return blaster[3][1]; }
float Blaster::getVeloX() { return veloX; }
float Blaster::getVeloY() { return veloY; }

void Blaster::setBlaster(glm::mat4 obj) { blaster = obj; }
void Blaster::setActive(bool var) { active = var; }
void Blaster::setAngle(float angle) { this->angle = angle; }
void Blaster::setVeloX(float x) { blaster[3][0] = x; }
void Blaster::setVeloY(float y) { blaster[3][1] = y; }
