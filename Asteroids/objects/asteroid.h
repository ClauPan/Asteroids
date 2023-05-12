#include "../dependente/glm/glm.hpp"
#include "../dependente/glm/gtc/matrix_transform.hpp"

class Asteroid {
private:
	glm::mat4 asteroid;
	float angle;
	float size;
	int lives;
	int hits;
	bool active;
	int spawn;
	float speed;

public:
	Asteroid();

	glm::mat4 getAsteroid();

	float getAngle();
	float getSize();
	float getPosX();
	float getPosY();
	float getSpeed();
	bool getActive();
	int getHits();
	int getLives();
	int getSpawn();
	float getSin();
	float getCos();

	void setAsteroid(glm::mat4 obj);

	void setAngle(float ang);
	void setSize(float s);
	void setActive(bool val);
	void setHits();
	void setLives(int val);
	void setSpawn(int val);
	void setSpeed(float val);
};
