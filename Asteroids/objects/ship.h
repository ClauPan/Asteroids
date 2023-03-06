#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"

class Ship {
public:
	glm::mat4 ship;
	int lives;
	int hits;

	Ship() {
		glm::mat4 ship(1.0f);
		lives = 3;
		hits = 0;
	}
	glm::mat4 getShip() {
		return ship;
	}
	float getPosX() {
		return ship[3][0];
	}
	float getPosY() {
		return ship[3][1];
	}
	int getHits() {
		return hits;
	}
	int getLives() {
		return lives;
	}
	void setShip(glm::mat4 obj) {
		ship = obj;
	}
	void setHits() {
		hits++;
	}
	void setHitsRestart() {
		hits=0;
	}
};

