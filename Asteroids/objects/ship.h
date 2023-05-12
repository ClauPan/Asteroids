#include "../dependente/glm/glm.hpp"
#include "../dependente/glm/gtc/matrix_transform.hpp"

class Ship {
private:
	glm::mat4 ship;
	int lives;
	int hits;

public:
	Ship();

	glm::mat4 getShip();

	float getPosX();
	float getPosY();
	int getHits();
	int getLives();

	void setShip(glm::mat4 obj);
	void setHits();
	void setHitsRestart();
};

