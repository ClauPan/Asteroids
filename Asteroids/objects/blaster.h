#include "../dependente/glm/glm.hpp"
#include "../dependente/glm/gtc/matrix_transform.hpp"
#include <math.h>
#define PI 3.14159265

class Blaster {
private:
	glm::mat4 blaster;
	float angle;
	bool active;
	float veloX;
	float veloY;

public:

	Blaster();

	float calculate_angle(glm::mat4 obj);
	glm::mat4 getBlaster();

	float getAngle();
	bool getActive();
	float getPosX();
	float getPosY();
	float getVeloX();
	float getVeloY();

	void setBlaster(glm::mat4 obj);
	void setActive(bool var);
	void setAngle(float angle);
	void setVeloX(float x);
	void setVeloY(float y);
};
