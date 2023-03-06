#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"

class Asteroid {
public:
	glm::mat4 asteroid;
	float angle;
	float size;
	int lives;
	int hits;
	bool active;
	int spawn;
	float speed;

	Asteroid() {
		glm::mat4 asteroid(1.0f);
		angle = 0.0f;
		size = 0.0f;
		hits = 0;
		spawn = 0;
		lives = 0;
		speed = 0;
		active = true;
	}
	glm::mat4 getAsteroid() {
		return asteroid;
	}
	float getAngle() {
		return angle;
	}
	float getSize() {
		return size;
	}
	float getPosX() {
		return asteroid[3][0];
	}
	float getPosY() {
		return asteroid[3][1];
	}
	float getSpeed() {
		return speed;
	}
	bool getActive() {
		return active;
	}
	int getHits() {
		return hits;
	}
	int getLives() {
		return lives;
	}
	int getSpawn() {
		return spawn;
	}
	float getSin() {
		return asteroid[0][1];
	}
	float getCos() {
		return asteroid[0][0];
	}
	void setAsteroid(glm::mat4 obj) {
		asteroid = obj;
	}
	void setAngle(float ang) {
		angle = ang;
	}
	void setSize(float s) {
		size = s;
	}
	void setActive(bool val) {
		active = val;
	}
	void setHits() {
		hits++;
	}
	void setLives(int val) {
		lives = val;
	}
	void setSpawn(int val) {
		spawn = val;
	}
	void setSpeed(float val) {
		speed = val;
	}
};
