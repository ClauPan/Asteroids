#include "ship.h"


Ship::Ship() {
	glm::mat4 ship(1.0f);
	lives = 3;
	hits = 0;
}

glm::mat4 Ship::getShip() { return ship; }

float Ship::getPosX() { return ship[3][0]; }
float Ship::getPosY() { return ship[3][1]; }
int Ship::getHits() { return hits; }
int Ship::getLives() { return lives; }

void Ship::setShip(glm::mat4 obj) { ship = obj; }
void Ship::setHits() { hits++; }
void Ship::setHitsRestart() { hits = 0; }
