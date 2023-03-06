#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"
#include <math.h>
#define PI 3.14159265

class Blaster {
public:
	glm::mat4 blaster;
	float angle;
	bool active;
	float veloX;
	float veloY;

	Blaster() {
		glm::mat4 blaster(1.0f);
		angle = 0.0f;
		active = false;
		veloX = 0.0f;
		veloY = 0.0f;
	}
	float calculate_angle(glm::mat4 obj) {
		float ac = (acos(obj[0][0])) * 180.0 / PI;
		float as = (asin(obj[0][1])) * 180.0 / PI;
		if (as < 0) { return -ac; }
		else { return ac; }
	}
	float getAngle() {
		return angle;
	}
	glm::mat4 getBlaster() {
		return blaster;
	}
	bool getActive() {
		return active;
	}
	float getPosX() {
		return blaster[3][0];
	}
	float getPosY() {
		return blaster[3][1];
	}
	float getVeloX() {
		return veloX;
	}
	float getVeloY() {
		return veloY;
	}
	void setBlaster(glm::mat4 obj) {
		blaster = obj;
	}
	void setActive(bool var) {
		active = var;
	}
	void setAngle(float angle) {
		this->angle = angle;
	}
	void setVeloX(float x) {
		blaster[3][0] = x;
	}
	void setVeloY(float y) {
		blaster[3][1] = y;
	}
};
