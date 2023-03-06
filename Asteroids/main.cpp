#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include "dependente/imgui/imgui.h"
#include "dependente/imgui/imgui_impl_glfw.h"
#include "dependente/imgui/imgui_impl_opengl3.h"
#include "dependente/glew/glew.h"
#include "dependente/glfw/glfw3.h"
#include "dependente/glm/glm.hpp"
#include "dependente/glm/gtc/matrix_transform.hpp"
#include "dependente/glm/gtc/type_ptr.hpp"
#include "shaders/shader.hpp"
#include "objects/blaster.h"
#include "objects/asteroid.h"
#include "objects/ship.h"

#define MAX_ASTEROIDS_PER_LEVEL 2
#define MAX_LEVEL 3
#define PI 3.14159265

//Window values
GLFWwindow* window;
int width = 1024, height = 1024;
float start_time = glfwGetTime();

//FILE READING
std::vector<std::string> Text;

//BOOL VALUES
bool moveFor = true;
bool moveBack = true;
bool startGame = false;
int gameStage = -1;
bool instructions = false;
bool hide_cursor = false;
bool level_change = false;
bool isItAtBeggining = true;

//TIMER 
clock_t start, end;
int deltaTime = 0;

//VARIABLES FOR LEVELS
static float asteroidSize[] = {0.15f, 0.3f, 0.45f};
static float asteroidSpeed[] = { -0.005, -0.003, -0.001 };
static int asteroidHits[] = { 2, 3, 4 };
static int spawnTime[] = { 9, 6, 3 };
int Level = 0;
int size; 
int asteroidKills = 0;
int asteroidKillsTotal = 0;
int blasterShots = 0;
int score = 0;
float spawnSpeed = 0.0f; // once max level is reached speed will increase


//OBJECTS ON SCREEN
Ship ship;
std::vector<Blaster>blasters;
std::vector<Asteroid>asteroids;

//BUFFERS
GLuint vboSHIP, vaoSHIP, iboSHIP;
GLuint vboASTEROID, vaoASTEROID, iboASTEROID;

//RANDOM GENERATIN FUNCTION
int randomGen(int max) { return rand() % max; }
void window_callback(GLFWwindow* window, int new_width, int new_height) {
	glViewport(0, 0, new_width, new_height);
	width = new_width;
	height = new_height;
}
//DRAW FUNCTION
void DRAW(GLuint Shader, GLuint vao, glm::mat4 obj, glm::vec4 color, int indices) {
	glUseProgram(Shader);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "transform"), 1, GL_FALSE, glm::value_ptr(obj));
	glUniform4fv(glGetUniformLocation(Shader, "color"), 1, glm::value_ptr(color));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
//CONTROL FUNCTIONS FOR OBJECTS ON WINDOW
void SHIP_CONTROLS() {
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
		ship.setShip(glm::rotate(ship.getShip(), 0.5f, glm::vec3(0.0, 0.0, 1.0)));
	}
	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
		ship.setShip(glm::rotate(ship.getShip(), -0.5f, glm::vec3(0.0, 0.0, 1.0)));
	}
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
		if (moveFor == true || moveBack==false) {
			ship.setShip(glm::translate(ship.getShip(), glm::vec3(+0.001f, 0.0f, 0.0)));
			moveBack = true;
		}
		if (ship.getPosX() > 0.96f || ship.getPosY() > 0.96f || ship.getPosX() < -0.96f || ship.getPosY() < -0.96f) {
			moveFor = false;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
		if (moveBack == true || moveFor==false) {
			ship.setShip(glm::translate(ship.getShip(), glm::vec3(-0.001f, 0.0f, 0.0)));
			moveFor = true;
		}
		if (ship.getPosX() > 0.96f || ship.getPosY() > 0.96f || ship.getPosX() < -0.96f || ship.getPosY() < -0.96f) {
			moveBack = false;
		}
	}
}
void BLASTER_SPAWN() {
	Blaster blaster;
	blaster.setActive(true);
	blaster.setBlaster(glm::translate(blaster.getBlaster(), glm::vec3(ship.getPosX(), ship.getPosY(), 0.0f)));
	float angle = blaster.calculate_angle(ship.getShip());
	blaster.setAngle(angle);
	blaster.setBlaster(glm::rotate(blaster.getBlaster(), angle, glm::vec3(0.0, 0.0, 1.0)));
	blaster.setBlaster(glm::scale(blaster.getBlaster(), glm::vec3(0.45f, 0.2f, 0.0f)));
	blasters.push_back(blaster);
	
}
void BLASTER_FIRE()
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		BLASTER_SPAWN();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		BLASTER_SPAWN();
		
}
void BLASTER_RAPID_FIRE() {
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		BLASTER_SPAWN();
	}
}
void BLASTER_CONTROLS() {
	for (int i = 0; i < blasters.size(); i++) {
		bool active = blasters[i].getActive();
		if (active == true) {
			blasters[i].setBlaster(glm::translate(blasters[i].getBlaster(), glm::vec3(0.005f, 0.0f, 0.0f)));
		}
		float x = blasters[i].getPosX();
		float y = blasters[i].getPosY();
		if (active == true && x < -1.2f || x > 1.2f || y < -1.2f || y > 1.2f) {
			blasters[i].setActive(false);
		}
		if (active == false) {
			blasters.erase(blasters.begin() + i);
		}
			
	}
}
void ASTEROID_SPAW(int level) {
	if (level == 1) { size = randomGen(2); }
	else if (level == 2) { size = randomGen(2) + 1; }
	else { size = 0; }
	float posX, posY;
	if (asteroids.empty() == true && deltaTime == 10 && isItAtBeggining==true) {
		Asteroid asteroid;
		float angle = float(randomGen(360));

		if (cos(angle * PI / 180) < 0) { posX = cos(angle * PI / 180) - 0.3; }
		else { posX = cos(angle * PI / 180) + 0.3; }
		if (sin(angle * PI / 180) < 0) { posY = sin(angle * PI / 180) - 0.3; }
		else { posY = sin(angle * PI / 180) + 0.3; }

		asteroid.setAngle(angle);
		asteroid.setSize(asteroidSize[size]);
		asteroid.setLives(asteroidHits[size]);
		asteroid.setSpeed(asteroidSpeed[size]);
		asteroid.setSpawn(spawnTime[level]);

		asteroid.setAsteroid(glm::translate(asteroid.getAsteroid(), glm::vec3(posX, posY, 0.0f)));
		asteroid.setAsteroid(glm::rotate(asteroid.getAsteroid(), angle, glm::vec3(0.0, 0.0, 1.0)));
		asteroid.setAsteroid(glm::scale(asteroid.getAsteroid(), glm::vec3(asteroidSize[size], asteroidSize[size], 0.0f)));

		asteroids.push_back(asteroid);
		start = end;
		isItAtBeggining = false;
	}
	if (deltaTime == spawnTime[level]) {
		Asteroid asteroid;
		float angle = float(randomGen(360));

		if (cos(angle * PI / 180) < 0) { posX = cos(angle * PI / 180) - 0.2; }
		else { posX = cos(angle * PI / 180) + 0.2; }
		if (sin(angle * PI / 180) < 0) { posY = sin(angle * PI / 180) - 0.2; }
		else { posY = sin(angle * PI / 180) + 0.2; }

		asteroid.setAngle(angle);
		asteroid.setSize(asteroidSize[size]);
		asteroid.setLives(asteroidHits[size]);
		asteroid.setSpeed(asteroidSpeed[size]);
		asteroid.setSpawn(spawnTime[level]);

		asteroid.setAsteroid(glm::translate(asteroid.getAsteroid(), glm::vec3(posX, posY, 0.0f)));
		asteroid.setAsteroid(glm::rotate(asteroid.getAsteroid(), angle, glm::vec3(0.0, 0.0, 1.0)));
		asteroid.setAsteroid(glm::scale(asteroid.getAsteroid(), glm::vec3(asteroidSize[size], asteroidSize[size], 0.0f)));

		asteroids.push_back(asteroid);
		start = end;
	}
	for (int i = 0; i < asteroids.size(); i++) {
		bool active = asteroids[i].getActive();
		if (active == true) {
			asteroids[i].setAsteroid(glm::translate(asteroids[i].getAsteroid(), glm::vec3(asteroids[i].getSpeed()-spawnSpeed, 0.0f, 0.0f)));
		}
		float x = asteroids[i].getPosX();
		float y = asteroids[i].getPosY();
		if (active == true && x < -1.2f || x > 1.2f || y < -1.2f || y > 1.2f)
			asteroids[i].setActive(false);
		if (active == false)
			asteroids.erase(asteroids.begin() + i);
	}
}
void SHIP_COLISSION() {
	for (int i = 0; i < asteroids.size(); i++) {
		float asteroidPosX = asteroids[i].getPosX();
		float asteroidPosY = asteroids[i].getPosY();
		float asteroidSize = asteroids[i].getSize();
		float asteroidLimitXL = asteroidPosX - asteroidSize / 4;
		float asteroidLimitXR = asteroidPosX + asteroidSize / 4;
		float asteroidLimitYL = asteroidPosY - asteroidSize / 4;
		float asteroidLimitYR = asteroidPosY + asteroidSize / 4;

		if (ship.getPosX() <= asteroidLimitXR && ship.getPosX() >= asteroidLimitXL) {
			if (ship.getPosY() <= asteroidLimitYR && ship.getPosY() >= asteroidLimitYL) {
				ship.setHits();
				asteroids[i].setActive(false);
			}
		}
		if (ship.getLives() == ship.getHits()) { 
			startGame = false; 
			asteroids.clear();
			blasters.clear();
		}
	}
}
void ASTEROID_COLLISION() {
	for (int i = 0; i < asteroids.size(); i++) {
		float asteroidPosX = asteroids[i].getPosX();
		float asteroidPosY = asteroids[i].getPosY();
		float asteroidSize = asteroids[i].getSize();
		float asteroidLimitXL = asteroidPosX - asteroidSize / 4;
		float asteroidLimitXR = asteroidPosX + asteroidSize / 4;
		float asteroidLimitYL = asteroidPosY - asteroidSize / 4;
		float asteroidLimitYR = asteroidPosY + asteroidSize / 4;

		for (int j = 0; j < blasters.size(); j++) {
			if (blasters[j].getPosX() <= asteroidLimitXR && blasters[j].getPosX() >= asteroidLimitXL) {
				if (blasters[j].getPosY() <= asteroidLimitYR && blasters[j].getPosY() >= asteroidLimitYL) {
					asteroids[i].setHits();
					blasters[j].setActive(false);
					blasterShots++;
					score += 20;
					break;
				}
			}
		}
		if (asteroids[i].getLives() == asteroids[i].getHits()) {
			asteroids[i].setActive(false); 
			asteroidKills++; 
			asteroidKillsTotal ++;
		}
	}
}
//CHECKING FUCTIONS
void INSTRUCTIONS() {
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (instructions == false) {
			instructions = true;
		}
		else {
			instructions = false;
		}
	}
}
void HIDE(){
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (hide_cursor == false) {
			hide_cursor = true; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			hide_cursor = false; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
void CHECK_START() {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { 
		startGame = true; 
		gameStage = 0; 
		asteroidKillsTotal = 0;
		score = 0;
		Level = 0;
		blasterShots = 0;
		asteroids.clear();
		blasters.clear();
		ship.setShip(glm::mat4(1.0f));
		ship.setHitsRestart();
		isItAtBeggining = true;
		spawnSpeed = 0.0f;
		start = clock();
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		startGame = true;
		gameStage = 0;
		asteroidKillsTotal = 0;
		score = 0;
		Level = 0;
		blasterShots = 0;
		asteroids.clear();
		blasters.clear();
		ship.setShip(glm::mat4(1.0f));
		ship.setHitsRestart();
		isItAtBeggining = true;
		spawnSpeed = 0.0f;
		start = clock();
	}
}
//KEY & BUTTON CALLBACKS
void KEY_CALLBACKS(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	INSTRUCTIONS();
	HIDE();
	BLASTER_FIRE();
	CHECK_START();
	
}
void MOUSE_BUTTON_CALLBACKS(GLFWwindow* window, int button, int action, int mods) {
	BLASTER_FIRE();
	CHECK_START();
}

//TEXT STYLER
void TextCenter(const char* text, int indice, bool startMiddle, int variable, bool variableSent)
{
	if (startMiddle == true && variableSent == false) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() + 100 * indice) / 4.f);
		ImGui::Text(text);
	}
	if (variableSent == true && startMiddle == false) {
		ImGui::SameLine(0.0f, 50.0f);
		ImGui::Text("%s   %d", text, variable);
	}
	if (variableSent == false && startMiddle == false) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth()) / 4.f);
		ImGui::Text(text);
	}
	if (variableSent == true && startMiddle == true) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() + 100 * indice) / 4.f);
		ImGui::Text("%s   %d", text, variable);
	}
}
void READFILE(std::string fileName) {
	// Create a text string, which is used to output the text file
	std::string line;

	// Read from the text file
	std::ifstream File(fileName);

	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(File, line)) {
		Text.push_back(line);
	}

	// Close the file
	File.close();
}


//////////////////////////////////////////////////////////  GAME   ///////////////////////////////////////////////////////
void START_GAME() {
	if (startGame == true) {
		SHIP_CONTROLS();
		glfwSetMouseButtonCallback(window, MOUSE_BUTTON_CALLBACKS);
		glfwSetKeyCallback(window, KEY_CALLBACKS);
		BLASTER_RAPID_FIRE();
		BLASTER_CONTROLS();
		end = clock();
		deltaTime = (end - start) / CLOCKS_PER_SEC;
		ASTEROID_SPAW(Level);
		SHIP_COLISSION();
		ASTEROID_COLLISION();
		if (Level < MAX_LEVEL-1) {
			if (asteroidKills == MAX_ASTEROIDS_PER_LEVEL+Level) { 
				Level++; 
				level_change = true;
				asteroidKills = 0;
			}
		}
		else if (Level==2){
			if (asteroidKills == MAX_ASTEROIDS_PER_LEVEL + Level) { spawnSpeed += 0.002f; asteroidKills = 0; }
		}
	}
	else { gameStage = 1; }
}

/////////////////////////////////////////////////////////   MAIN   ///////////////////////////////////////////////////////
int main(void) {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//SHADERS
	GLuint SpaceShip = LoadShaders("shaders/vertex/ship.vertexshader", "shaders/fragment/ship.fragmentshader");
	GLuint Blaster = LoadShaders("shaders/vertex/blaster.vertexshader", "shaders/fragment/blaster.fragmentshader");
	GLuint Asteroid = LoadShaders("shaders/vertex/asteroids.vertexshader", "shaders/fragment/asteroids.fragmentshader");

	//ARRAYS
	GLfloat ship_vertices[] = {
		0.0f, 0.025f, 0.0f,
		0.06f, 0.0f, 0.0f,
		0.0f, -0.025f, 0.0f,
	};
	GLuint ship_indices[] = {
		0, 1, 2,
	};

	GLfloat asteroid_vertices[] = {
		0.0f, 0.0f, 0.0f,
		0.2f, 0.0f, 0.0f,
		0.1f, 0.173f, 0.0f,
		-0.1f, 0.173f, 0.0f,
		-0.2f, 0.0f, 0.0f,
		-0.1f, -0.173f, 0.0f,
		0.1f, -0.173f, 0.0f
	};
	GLuint asteroid_indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 1
	};

	//BUFFERS
	glGenVertexArrays(1, &vaoSHIP);//BUFFER FOR SHIP AND BLASTER
	glGenBuffers(1, &vboSHIP);
	glGenBuffers(1, &iboSHIP);
	glBindVertexArray(vaoSHIP);
	glBindBuffer(GL_ARRAY_BUFFER, vboSHIP);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ship_vertices), ship_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboSHIP);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ship_indices), ship_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &vaoASTEROID);//BUFFER FOR ASTEROIDS
	glGenBuffers(1, &vboASTEROID);
	glGenBuffers(1, &iboASTEROID);
	glBindVertexArray(vaoASTEROID);
	glBindBuffer(GL_ARRAY_BUFFER, vboASTEROID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(asteroid_vertices), asteroid_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboASTEROID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(asteroid_indices), asteroid_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glfwSetFramebufferSizeCallback(window, window_callback);//CALLBACK FOR WINDOW RESIZING

	//POSITIONING SHIP ON SCEEN AND STARTING CLOCK
	ship.setShip(glm::translate(ship.getShip(), glm::vec3(0.0f, 0.0f, 0.0f)));

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	while (!glfwWindowShouldClose(window)) {

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
		glfwSetKeyCallback(window, KEY_CALLBACKS);
		glfwSetMouseButtonCallback(window, MOUSE_BUTTON_CALLBACKS);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		//window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoResize;
		bool open_ptr = true;
		ImGuiCond cond = 0;
		
		if (instructions == true) {
			ImGui::SetNextWindowPos(ImVec2(width / 4, height / 4));
			ImGui::SetNextWindowSize(ImVec2(width / 2, height / 2), cond);
			READFILE("textFiles/Instructions.txt");
			ImGui::Begin("Commands Menu", &open_ptr, window_flags);
			for (int i = 0; i < Text.size(); i++) {
				TextCenter(Text[i].c_str(), i, false, 0, false);
			}
			Text.clear();
			ImGui::End();
		}
		else {
			if (gameStage == -1) {
				ImGui::SetNextWindowPos(ImVec2(width/4, height/4));
				ImGui::SetNextWindowSize(ImVec2(width/2, height/2), cond);
				READFILE("textFiles/Main_menu.txt");
				ImGui::Begin("Main Menu", &open_ptr, window_flags);
				for (int i = 0; i < Text.size(); i++) {
					TextCenter(Text[i].c_str(), i, true, 0, false);
				}
				Text.clear();
				ImGui::End();
			}
		}
		if (gameStage == 0) {
			//GAME BEGINS
			START_GAME();
			
			//DRAWING OBECTS ON WINDOW
			for (int i = 0; i < blasters.size(); i++) {
				if (blasters[i].getActive() == true)
					DRAW(Blaster, vaoSHIP, blasters[i].getBlaster(), glm::vec4(1.0f, 1.0f, 0.0f, 1.0), 3);
			}
			DRAW(SpaceShip, vaoSHIP, ship.getShip(), glm::vec4(0.5f, 0.0f, 0.5f, 1.0), 3);
			for (int i = 0; i < asteroids.size(); i++) {
				if (asteroids[i].getActive() == true)
					DRAW(Asteroid, vaoASTEROID, asteroids[i].getAsteroid(), glm::vec4(0.5f, 0.5f, 0.5f, 1.0), 18);
			}
			ImGui::SetNextWindowPos(ImVec2(width / 4 / 2 / 2, 30));
			ImGui::SetNextWindowSize(ImVec2((width / 4 / 2 / 2) * 14, 50), cond);
			READFILE("textFiles/HUD.txt");
			ImGui::Begin("HUD", &open_ptr, window_flags);
			for (int i = 0; i < Text.size(); i++) {
				if (i == 0)
					TextCenter(Text[i].c_str(), i, false, score, true);
				if (i == 1)
					TextCenter(Text[i].c_str(), i, false, blasterShots, true);
				if (i == 2)
					TextCenter(Text[i].c_str(), i, false, asteroidKillsTotal, true);
				if (i == 3)
					TextCenter(Text[i].c_str(), i, false, Level + 1, true);
				if (i == 4)
					TextCenter(Text[i].c_str(), i, false, ship.getLives()-ship.getHits(), true);
			}
			Text.clear();
			ImGui::End();
		}
		if (gameStage == 1) {
			ImGui::SetNextWindowPos(ImVec2(width / 4, height / 4));
			ImGui::SetNextWindowSize(ImVec2(width / 2, height / 2), cond);
			READFILE("textFiles/Exit_menu.txt");
			ImGui::Begin("Exit Menu", &open_ptr, window_flags);
			for (int i = 0; i < Text.size(); i++) {
				if (i == 4) { TextCenter(Text[i].c_str(), i, true, score, true); }
				else if (i == 6) { TextCenter(Text[i].c_str(), i, true, blasterShots, true); }
				else if (i == 8) { TextCenter(Text[i].c_str(), i, true, asteroidKillsTotal, true); }
				else { TextCenter(Text[i].c_str(), i, true, 0, false); }
			}
			Text.clear();
			ImGui::End();
		}
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glDeleteBuffers(1, &vboSHIP);
	glDeleteBuffers(1, &iboSHIP);
	glDeleteVertexArrays(1, &vaoSHIP);
	glDeleteBuffers(1, &vboASTEROID);
	glDeleteBuffers(1, &iboASTEROID);
	glDeleteVertexArrays(1, &vaoASTEROID);
	glDeleteProgram(SpaceShip);
	glDeleteProgram(Blaster);
	glDeleteProgram(Asteroid);
	glfwTerminate();
	return 0;
}


