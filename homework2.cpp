/*********
   CTIS164 - Homework2
----------
STUDENT : Onurcan Genç
SECTION : 4
HOMEWORK: Destroying Saturn Planets
----------
PROBLEMS: If you start game current speed does not increase itself, but if you press bullet trigger keys which are 'r', 'o' or 'l', it immmediately increases itself.
		  After the refresh 'F1' key it works properly !
		  You can always switch pause screen without the mode of the screen.
		  For instance: if you end game and press 'p' key for pausing, it pauses every time without depended the mode. Even if the game ends.


----------
ADDITIONAL FEATURES: If you press 'r', 'l' and 'o' keys, each of them represents different gun modes.
					 For using Red Gun as known as <Sub-Machine>: Press 'r' on your keyboard.
					 For using Lilac Gun as known as <Pistol Category>: Press 'l' on your keyboard.
					 For using Orange Gun as known as <Rifle>: Press 'o' on your keyboard.

					 Star shapes added on pause screen and it moves on X axis beyond the screen
					 Star shapes have randomized colors and fixed size
					 Heart Shape added at the end screen ! ! ! (you have to wait 20 seconds)

					 SATURN Planet & its eliptic circle added ! ! !
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200   
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  40 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


typedef struct planet {

	int planetX = rand() % 250 + 100,
		planetY = 0;

	bool appear = false;

}planet_t;

typedef struct color {

	int R = rand() % 255;
	int G = rand() % 255;
	int B = rand() % 255;

}color_t;

typedef struct star {

	int x;
	int y;
	color_t color;

}star_t;

/* Global Variables for Template File */
bool up = false,
down = false,
right = false,
left = false,
gameover = false,
orangeFire = false,
orangeLevel = false,
lilacFire = false,
lilacLevel = false,
redFire = false,
redLevel = false,
end = false,
pause = false;

int  winWidth, winHeight; // current Window width and height

int starX = 0, starY = 0; //Star Gun Locations

planet_t planet[100]; // Planet coordinates

int planetCount = 100; // Number of Planets

// Fire each type using according keys
/*   purpleFire // use 'p'
	 lilacFire // use 'l'
	 redFire  // use 'r'  */

int cannonX = 0, size = 30; // Laser Bullet 1
int cannonX2 = 0, size2 = 30; // Laser Bullet 2
int cannonX3 = 0, size3 = 30; // Laser Bullet 3

float gametime = 400.0; // Game Duration 20 second

//Parameter for counting and calculating point parameters
int redCount = 0, // Calculate number of shooting records for red gun which corresponds 1 points
orangeCount = 0, // Calculate number of shooting records for orange gun which corresponds 5 points
lilacCount = 0, // Calculate number of shooting records for lilac gun which corresponds 3 points
totalPlanet, // Total Planets count

// Variable for current speed parameter // Default Speed 1
redSpeed = 3, // Red gun speed parameter
orangeSpeed = 5, // Orange gun gun speed parameter
lilacSpeed = 2, // Lilac gun speed parameter
totalSpeed = 1, // total speed which corresponds initially
defaultSpeed = 1,
defaultCounter = 1;  // default Speed

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}


void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}



void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void draw_elliptical_line(int x, int y, int a, int b, float theta) {

	float angle, x1, y1, x2, y2;

	glLineWidth(4);

	glBegin(GL_LINES);
	for (int i = 0; i < 360; i += 10) {
		angle = (theta + i) * PI / 180;
		x1 = x + a * cos(angle);
		y1 = y + b * sin(angle);
		angle = (theta + i + 10) * PI / 180;
		x2 = x + a * cos(angle);
		y2 = y + b * sin(angle);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	}
	glEnd();

}

void drawPlanet(planet_t* planet, int planetR) {

	glColor3f(0.82f, 0.69f, 0.41f);
	circle(planet->planetX, planet->planetY, planetR);

	glColor3f(0.86f, 0.84f, 0.80f);
	draw_elliptical_line(planet->planetX, planet->planetY, planetR + 30, planetR - 30, 30.0);


	glColor3f(0.51f, 0.13f, 0.05f);
	circle(planet->planetX - 38, planet->planetY + 10, planetR - 45);
	circle(planet->planetX - 40, planet->planetY - 10, planetR - 45);
	circle(planet->planetX - 30, planet->planetY - 30, planetR - 45);
	circle(planet->planetX - 10, planet->planetY - 40, planetR - 45);
	circle(planet->planetX + 10, planet->planetY - 40, planetR - 45);
	circle(planet->planetX + 40, planet->planetY - 10, planetR - 45);
	circle(planet->planetX + 40, planet->planetY + 10, planetR - 45);
	circle(planet->planetX + 30, planet->planetY + 30, planetR - 45);
	circle(planet->planetX + 10, planet->planetY + 40, planetR - 45);
	circle(planet->planetX - 10, planet->planetY + 40, planetR - 45);


}


void triangle(int x, int y, int x2, int y2, int x3, int y3) {


	glBegin(GL_TRIANGLES);
	glColor3ub(209, 64, 9); // orange
	glVertex2f(x, y);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();


}

void pauseScreen(star_t *stars) {


	glColor3ub(75, 0, 130);
	glRectf(-600, -400, 600, 400);
	glColor3ub(255, 255, 255);
	vprint(-250, 0, GLUT_BITMAP_8_BY_13, "Press 'p' key on your keyboard, If you wish to continue !");
	vprint(-160, 100, GLUT_BITMAP_8_BY_13, "Press 'ESC' key to end the game !");
	vprint(-170, 200, GLUT_BITMAP_8_BY_13, "Are you sure about pausing the game ?");

	// Adjust the size of the stars
	glBegin(GL_POINT_SIZE);
	// 5.0 for floating number size
	glPointSize(3.0);
	//start draw stars for pause screen
	glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++) {
		stars[i].x = (rand() % 1201) - 600;
		stars[i].y = (rand() % 800) - 400;
		glColor3ub(stars[i].color.R, stars[i].color.G, stars[i].color.B);
		glVertex2f(stars[i].x, stars[i].y);
	}
	glEnd();
}

void gameEnd() {

	glColor3ub(0, 0, 0);
	glRectf(-600, -400, 600, 400);
	glColor3ub(255, 255, 255);
	vprint(-100, 0, GLUT_BITMAP_8_BY_13, "! ! ! CTIS164 WINS ! ! !");
	vprint(-120, 100, GLUT_BITMAP_8_BY_13, "Press 'ESC' key to end the game !");
	vprint(-100, 200, GLUT_BITMAP_8_BY_13, "- - F A T A L I T I Y - -");


	//Draw Heart
	glBegin(GL_POLYGON);
	float scale = size / 10.0;
	for (int i = 0; i < 360; i += 2) {
		float angle = i * 3.14159 / 180.0;
		float radius = scale * (16 * pow(sin(angle), 3));
		float posX = 0 + radius;
		float posY = -100 + scale * (-13 * cos(angle) + 5 * cos(2 * angle) + 2 * cos(3 * angle) + cos(4 * angle));
		glVertex2f(posX, posY);
	}
	glEnd();


}

void cannonLight(float x, float y, float size)
{

	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for the laser
	glLineWidth(5.0f); // Set the width of the laser beam
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(size, 0.0f);
	glEnd();
	glPopMatrix();
}

void cannonLight2(float x, float y, float size)
{

	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(2.0f, 0.5f, 1.0f); // Lilac color for the laser
	glLineWidth(5.0f); // Set the width of the laser beam
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(size, 0.0f);
	glEnd();
	glPopMatrix();
}

void cannonLight3(float x, float y, float size)
{

	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(1.0f, 0.5f, 0.0f); // Orange color for the laser
	glLineWidth(5.0f); // Set the width of the laser beam
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(size, 0.0f);
	glEnd();
	glPopMatrix();
}

void background() {

	glColor3ub(45, 89, 112);
	glRectf(-600, -400, 600, 400);

}


void drawHeart(float x, float y, float size) {
	glBegin(GL_POLYGON);
	float scale = size / 10.0;
	for (int i = 0; i < 360; i += 2) {
		float angle = i * 3.14159 / 180.0;
		float radius = scale * (16 * pow(sin(angle), 3));
		float posX = x + radius;
		float posY = y + scale * (-13 * cos(angle) + 5 * cos(2 * angle) + 2 * cos(3 * angle) + cos(4 * angle));
		glVertex2f(posX, posY);
	}
	glEnd();
}




//
// To display onto window using OpenGL commands
void window()
{

	// Point Chart & Starting - Pausing game utility

	glColor3ub(0, 0, 0);
	glRectf(-478, -307, +478, -500);


}

void sky() {

	//Sky
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.1f, 0.3f);
	glVertex2i(475, -275);
	glVertex2i(-475, -275);
	glColor3f(0.0f, 0.2f, 0.5f);
	glVertex2i(-475, 275);
	glVertex2i(475, 275);
	glEnd();

}

void whiteRectangle() {

	//white
	glColor3ub(255, 255, 255);
	glRectf(480, -280, -480, 280);

	glColor3ub(255, 255, 255);
	glRectf(505, -305, -505, 305);

	//brown
	glColor3ub(102, 51, 0);
	glRectf(500, -300, -500, +300);
}


void blackBox() {

	glColor3ub(157, 109, 60);
	glRectf(150, +320, -150, +380);
	glColor3f(1, 1, 1);
	vprint(-60, 370, GLUT_BITMAP_8_BY_13, "**Onurcan Genc**");
	vprint(-35, 350, GLUT_BITMAP_8_BY_13, "<22003467>");
	vprint(-60, 330, GLUT_BITMAP_8_BY_13, "Destroy Planets");
}

void lines() {

	glColor3ub(157, 109, 60);
	glRectf(-505, -400, -478, 400);

	glColor3ub(157, 109, 60);
	glRectf(478, -400, 505, 400);

}


void drawStarGun(int starX, int starY)
{

	// The Star Gun will throw lasers to destroy planets :)

	//Mid Gun Barrel
	glColor3f(0.8f, 0.8f, 0.6f);
	glRectf(starX - 425, starY - 5, starX - 385, starY + 5);

	//Left Gun Barrel
	glColor3f(0.8f, 0.8f, 0.6f);
	glRectf(starX - 425, starY + 20, starX - 385, starY + 30);

	//Right Gun Barrel
	glColor3f(0.8f, 0.8f, 0.6f);
	glRectf(starX - 425, starY - 30, starX - 385, starY - 20);

	// Starting implementation of Gun Barrel
	glColor3ub(255, 128, 0);
	glRectf(starX - 435, starY - 40, starX - 425, starY + 40);

	// Bottom Side of the Interest Gun
	glColor3ub(0, 0, 0);
	glRectf(starX - 475, starY - 60, starX - 435, starY + 60);


}



void display() {
	//
	// clear window to black
	//

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	background();
	lines();
	window();


	if (!gameover && pause == false)
	{
		window();
		glColor3ub(255, 255, 255);
		vprint(-100, -370, GLUT_BITMAP_8_BY_13, "Press <F1> to start a new game");
		glColor3ub(255, 0, 0);
		vprint(-300, -370, GLUT_BITMAP_8_BY_13, "Red SHOT: Press 'r'");
		glColor3ub(255, 165, 0);
		vprint(-50, -330, GLUT_BITMAP_8_BY_13, "Orange SHOT: Press 'o'");
		glColor3ub(200, 162, 200);
		vprint(200, -370, GLUT_BITMAP_8_BY_13, "Lilac SHOT: Press 'l'");
	}

	if (gameover == true && pause == false)
	{

		drawStarGun(starX, starY);

		if (redFire == true) {

			cannonLight(cannonX - 380, starY + 25, size + 5);


			if (cannonX > 800) {
				cannonX = starX;
				redFire = false;
			}
		}


		if (lilacFire == true) {

			cannonLight2(cannonX3 - 380, starY - 25, size3 + 5);

			if (cannonX3 > 800) {
				cannonX3 = starX;
				lilacFire = false;
			}

		}


		if (orangeFire == true) {

			cannonLight3(cannonX2 - 380, starY, size2 + 5);


			if (cannonX2 > 800) {
				cannonX2 = starX;
				orangeFire = false;
			}
		}



		for (int i = 0; i < planetCount; i++) {


			if (planet[i].appear == true) {

				drawPlanet((planet + i), 50);

				if (planet[i].planetY >= -385)
					totalPlanet = i;
			}

		}

		blackBox();


		glColor3ub(0, 0, 0);
		glRectf(-478, -307, +478, -500);

		//DISPLAY NECESSARY POINTS & CALCULATIONS

		//TIME
		glColor3ub(255, 255, 255);
		vprint(0, -320, GLUT_BITMAP_8_BY_13, "%.2f", gametime / 20);
		vprint(-40, -340, GLUT_BITMAP_8_BY_13, "REMAINING TIME");
		vprint(-100, -370, GLUT_BITMAP_8_BY_13, "Press F1  to start a new game !");

		//RED SHOT DISPLAY
		glColor3ub(255, 0, 0);
		vprint(-450, -320, GLUT_BITMAP_8_BY_13, "Red SHOT:");
		glColor3ub(255, 255, 255);
		vprint(-380, -320, GLUT_BITMAP_8_BY_13, " %d", redCount);
		glColor3ub(255, 0, 0);
		vprint(-350, -320, GLUT_BITMAP_8_BY_13, "Red Point:");
		glColor3ub(255, 255, 255);
		vprint(-270, -320, GLUT_BITMAP_8_BY_13, " %d", redCount * 1);

		//ORANGE SHOT DISPLAY
		glColor3ub(255, 165, 0);
		vprint(-450, -350, GLUT_BITMAP_8_BY_13, "Orange SHOT:");
		glColor3ub(255, 255, 255);
		vprint(-360, -350, GLUT_BITMAP_8_BY_13, " %d", orangeCount);
		glColor3ub(255, 165, 0);
		vprint(-330, -350, GLUT_BITMAP_8_BY_13, "Orange Point:");
		glColor3ub(255, 255, 255);
		vprint(-230, -350, GLUT_BITMAP_8_BY_13, " %d", orangeCount * 5);

		//LILAC DISPLAY
		glColor3ub(200, 162, 200);
		vprint(-450, -390, GLUT_BITMAP_8_BY_13, "Lilac SHOT:");
		glColor3ub(255, 255, 255);
		vprint(-370, -390, GLUT_BITMAP_8_BY_13, " %d", lilacCount);
		glColor3ub(200, 162, 200);
		vprint(-340, -390, GLUT_BITMAP_8_BY_13, "Lilac POINT:");
		glColor3ub(255, 255, 255);
		vprint(-250, -390, GLUT_BITMAP_8_BY_13, " %d", lilacCount * 3);

		//TOTAL PLANETS && TOTAL POINTS && CURRENT SPEED
		glColor3ub(135, 206, 250);
		vprint(250, -390, GLUT_BITMAP_8_BY_13, "TOTAL POINTS:");
		glColor3ub(255, 255, 255); 
		vprint(360, -390, GLUT_BITMAP_8_BY_13, "%d ", redCount * 1 + orangeCount * 5 + lilacCount * 3);
		glColor3ub(0, 255, 202);
		vprint(250, -365, GLUT_BITMAP_8_BY_13, "CURRENT BULLET SPEED:");
		glColor3ub(255, 255, 255);
		vprint(420, -365, GLUT_BITMAP_8_BY_13, "%d ", (totalSpeed + defaultSpeed) / 2000);
		glColor3ub(135, 206, 250);
		vprint(250, -340, GLUT_BITMAP_8_BY_13, "TOTAL PLANETS:");
		glColor3ub(135, 206, 250);
		vprint(370, -340, GLUT_BITMAP_8_BY_13, "%d", totalPlanet);

		if (end == true && pause == false)
		{
			gametime = 400;
			gameEnd();
		}
	}

	else
		if (pause == true) {
			//initiliaze the star structure for drawing !
			star_t stars[100];
			pauseScreen(stars);
		}
			
	

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{

	switch (key)
	{
	case 'r':
		redFire = true;
		redLevel = true;
		break;
	case 'l':
		lilacFire = true;
		lilacLevel = true;
		break;
	case 'o':
		orangeFire = true;
		orangeLevel = true;
		break;
	case 'p':
		pause = !pause;
		break;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = true;
		starY += 10;
		break;
	case GLUT_KEY_DOWN:
		down = true;
		starY -= 10;
		break;
	case GLUT_KEY_F1:
		gameover = true;

		totalPlanet = 0;
		gametime = 400; // refresh the game
		redCount = 0; // refresh number of red bullet
		orangeCount = 0; // refresh number of orange bullet
		lilacCount = 0; // refresh number of lilac bullet
		totalSpeed = 1; // refresh speed parameter

		for (int i = 0; i < planetCount; i++) {

			planet_t newt;
			planet[i] = newt;
			planet[i].planetY = -360 - 120 * i;
			planet[i].appear = true;

		}

		break;
	case GLUT_KEY_RIGHT:
		right = true;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}



//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (gametime <= 0 && pause == false)
	{
		end = true;
		gametime = 400;

	}

	else if (!end && pause == false)
	{
		gametime -= 0.4;
	}


	if (end == false && pause == false)
	{

		if (starY > 350)
			starY = 350;

		if (starY < -215)
			starY = -215;


		if (redFire == true) {
			cannonX += 50;
		}

		if (lilacFire == true) {
			cannonX3 += 40;
		}

		if (orangeFire == true) {
			cannonX2 += 20;
		}

		for (int i = 0; i < planetCount; i++) {

			planet[i].planetY += defaultSpeed;
			defaultCounter++;

			if (redLevel)
			{
				//increased planet speed for Red Sub-Machine Rifle Gun
				planet[i].planetY += redSpeed;

				//total speed calculation
				totalSpeed += redSpeed;

				if (redFire && abs(cannonX - 380 - planet[i].planetX) <= 20 && abs(starY + 25 - planet[i].planetY) <= 100) {
					planet[i].appear = false;
					redFire == false;
					starY = starY;
					redCount++;
				}
			}

			if (orangeLevel)
			{

				//increased planet speed for Orange Sniper
				planet[i].planetY += orangeSpeed;

				//total speed calculation
				totalSpeed += orangeSpeed;

				if (orangeFire && abs(cannonX2 - 380 - planet[i].planetX) <= 20 && abs(starY - planet[i].planetY) <= 100)
				{
					planet[i].appear = false;
					orangeFire == false;
					starY = starY;
					orangeCount++;
				}
			}

			if (lilacLevel)
			{

				//increased planet speed for Orange Sniper since the galaxy is different
				planet[i].planetY += lilacSpeed;

				//total speed calculation
				totalSpeed += lilacSpeed;

				if (lilacFire && abs(cannonX3 - 380 - planet[i].planetX) <= 30 && abs(starY - 25 - planet[i].planetY) <= 100)
				{
					planet[i].appear = false;
					starY = starY;
					lilacFire == false;
					lilacCount++;
				}
			}


		}
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);

	srand(time(NULL));

	glutCreateWindow("UFO Graphics Corp.");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 1);
#endif

	Init();

	glutMainLoop();
}