/*********
CTIS164 - Template Source Program
----------
STUDENT :Umid Targuliyev
SECTION :01
HOMEWORK: shooting the ufos
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/



/*
 Improvements -> #1 car turns to side in which you pressed by button
 #2 in last 5 seconds time turns to red color as an alert

*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>



#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 900

#define TIMER_PERIOD  25 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


typedef struct {
	int x; //x value of moving objects
	int y; //y value of moving objects
	bool initialized; //it is used for giving random y vale for moving objects when they hit end or hit by rocket
	bool countChecker = false; //for counting total ufos
}ufo_t;

int carx = 0;
int rev = 1;


int rocketx = carx; //x value of shooted object
int rockety = 0;//y value of shooted object
bool shooted = false;//to check if object is shooted or not in order to move it becuase when it is not shooted it goes with same direction with car but when shooted it has its own x and y value
bool rocketFire = false; //fire appears and disappears


bool gameStart = false; //to start game with <F1>
ufo_t ufo[5];


int score = 0; // points gained by shooting ufos
int totalUfos = 0; // counts the amount of ufo shown up
int burstUfos = 0; // the number of shooted ufos 
int lastPoint = 0;//last point gained by shooting ufos




int timer = 2000;
bool gameStop = false;



/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

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
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
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
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);



void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
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
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//




void drawUfo(int x , int y){

	glColor3ub(72, 61, 139);
	circle(x, y - 10, 25);

	
	glBegin(GL_QUADS);
	glColor3ub(220, 220, 220);
	glVertex2f(-25 + x, y);
	glVertex2f(-75 + x, -35 + y);
	glColor3ub(105, 105, 105);
	glVertex2f(+75 + x, -35 + y);
	glVertex2f(+25 + x, y);
	glEnd();

	glColor3f(0, 0, 0);
	circle(-35 + x, -17.5 + y, 7);
	glColor3f(1, 0, 0);
	//glColor3ub(0, g, 0 );
	circle(-35 + x, -17.5 + y, 5);

	glColor3f(0, 0, 0);
	circle(-0 + x, -17.5 + y, 7);
	glColor3f(0, 1, 0);
	//glColor3ub(r, 0, 0);
	circle(-0 + x, -17.5 + y, 5);

	glColor3f(0, 0, 0);
	circle(+35 + x, -17.5 + y, 7);
	glColor3f(0, 0, 1);
	//glColor3ub(0, 0, b);
	circle(+35 + x, -17.5 + y, 5);
	
	
	
}



void fire(int x) {
	glColor3ub(255, 69, 0);
	circle(x, -245 + rockety, 10);
	glColor4f(1, 1, 0, 0.5);
	circle(x, -245 + rockety, 15);
}


void displayRocket(int x) {
	glColor3f(1, 0, 0);
	circle(x, -190 + rockety, 15); // head
	

	if (rocketFire)
		fire(x);

	glColor3ub(47, 79, 79);
	glRectf(-15 + x, -190 + rockety, +15 + x, -240 + rockety); // body



	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);//wings
//left
	glVertex2f(-15 + x, -215 + rockety);
	glVertex2f(-25 + x, -221 + rockety);
	glVertex2f(-25 + x, -230 + rockety);
	glVertex2f(-15 + x, -230 + rockety);
//right
	glVertex2f(+15 + x, -215 + rockety);
	glVertex2f(+25 + x, -221 + rockety);
	glVertex2f(+25 + x, -230 + rockety);
	glVertex2f(+15 + x, -230 + rockety);
	glEnd();
	
	glColor3ub(112, 128, 144);
	glBegin(GL_QUADS);
	glVertex2f(-15 + x, -240+ rockety);
	glVertex2f(-10 + x, -245+ rockety);
	glVertex2f(+10 + x, -245 + rockety);
	glVertex2f(+15 + x, -240 + rockety);
	glEnd();


	

}

	




void displayCar() {
	glColor3ub(0, 0, 0); //tire
	circle(-35 *rev + carx, -255, 20);
	circle(+35 * rev + carx, -255, 20);

	glColor3ub(248, 248, 255); //inside tire
	circle(-35  * rev + carx, -255, 10);
	circle(+35 * rev  + carx, -255, 10);


	glColor3ub(27, 77, 62);
	glRectf(-70 * rev + carx, -220, +70 * rev + carx, -260); // body
	glRectf(+60 * rev + carx, -225, +40 * rev + carx, -200); // front window base
	glColor3ub(138, 43, 226);
	glRectf(+56 * rev + carx, -220, +45 * rev + carx, -202); // window

	glColor3f(1, 1, 0);
	glRectf(+60 * rev + carx, -225, +70 * rev + carx, -230); // frontlight

	glColor3f(1, 0, 0);
	glRectf(-60 * rev + carx, -225, -70 * rev + carx, -230); // backlight

	glColor3f(0, 0, 0);
	glRectf(-40 * rev + carx, -190, +40 * rev + carx, -240);

	glRectf(-20 * rev + carx, -170, +20 * rev + carx, -240);
	




}


void displayCloud(int x, int y) {
	glColor3ub(240, 248, 255);
	circle(21 + x, -24 + y, 15);
	circle(25 + x, -7 + y, 25);
	circle(-25 + x, +5 + y, 25);
	circle(0 + x, 0 + y, 35);
}


void displayBackground() {
	
	//sky
	glBegin(GL_QUADS);
	glColor3ub(0, 191, 255);
	glVertex2f(-600, 450);
	glVertex2f(600, 450);
	glColor3ub(240, 248, 255);
	glVertex2f(600, -450);
	glVertex2f(-600, -450);
	glEnd();
	
	//soil
	glBegin(GL_QUADS);
	glColor3ub(255, 222, 173);
	glVertex2f(-600, -210);
	glVertex2f(600, -210);
	glColor3ub(139, 69, 19);
	glVertex2f(600, -450);
	glVertex2f(-600, -450);
	glEnd();


	displayCloud(450, 340);
	displayCloud(250, 300);
	displayCloud(0, 340);


	displayCloud(-250, 300);
	displayCloud(-450, 340);

}


void menu() {
	glColor3f(0, 0, 0);
	glRectf(500, 600, 600, -600);

	glLineWidth(2);
	glColor3f(1, 1, 0);

	vprint(510, 400, GLUT_BITMAP_HELVETICA_18, "Remaining");
	vprint(530, 375, GLUT_BITMAP_HELVETICA_18, "Time");
	
	if (timer > 0)
	{
		if (timer < 500)
		{
			glColor3f(1, 0, 0);
			if(timer % 5 != 0)
				vprint(530, 350, GLUT_BITMAP_9_BY_15, "%02d:%02d", timer / 100, timer % 100);
		}
		else
		{
			glColor3f(0, 1, 0);
			vprint(530, 350, GLUT_BITMAP_9_BY_15, "%02d:%02d", timer / 100, timer % 100);
		}
	}
	else
	{
		glColor3f(1, 0, 0);
		vprint(530, 350, GLUT_BITMAP_9_BY_15, "00:00");
	}
	glColor3f(1, 1, 0);

	vprint(510, 200, GLUT_BITMAP_HELVETICA_18, "Total Ufos");
	//vprint(510, 225, GLUT_BITMAP_HELVETICA_18, "Ufos");
	vprint(540, 175, GLUT_BITMAP_9_BY_15, "%02d", totalUfos);


	vprint(510, 0, GLUT_BITMAP_HELVETICA_18, "Burst Ufos");
	//vprint(510, -75, GLUT_BITMAP_TIMES_ROMAN_24, "Ufos");
	vprint(540, -25, GLUT_BITMAP_9_BY_15, "%02d",burstUfos);

	vprint(510, -100, GLUT_BITMAP_HELVETICA_18, "Last Points");
	//vprint(510, -175, GLUT_BITMAP_TIMES_ROMAN_24, "Points");
	vprint(540, -125, GLUT_BITMAP_9_BY_15, "%d", lastPoint);


	vprint(520, -250, GLUT_BITMAP_HELVETICA_18, "Score");
	vprint(525, -275, GLUT_BITMAP_9_BY_15, "%03d", score);




	
	if (!gameStart || gameStop)
		vprint(-550, -320, GLUT_BITMAP_9_BY_15, "Press <F1> to (re)start game");
	else
		vprint(-550, -320, GLUT_BITMAP_9_BY_15, "Press <F1> to stop game");
}

void display() {

	//
	// clear window to black
	//
	
	displayBackground();
	menu();

	
	if (!shooted)
		rocketx = carx;
	displayRocket(rocketx);
		


	if (rockety > 630) {
		shooted = false;
		rockety = 0;
		rocketFire = false;
	}

	displayCar();


	for (int i = 0; i < 5; i++)
		drawUfo(ufo[i].x, ufo[i].y);


	glutSwapBuffers();
	
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == ' ' && gameStart)
		shooted = true;


	if (key == 's' || key == 'S')
		gameStart = !gameStart;
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
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (key == GLUT_KEY_F1 && gameStop) {
		gameStop = false;
		for (int i = 0; i < 5; i++) {
			ufo[i].x = -675 - i * 300;
			ufo[i].initialized = false;
		}
		timer = 2000;
		totalUfos = 0;
		burstUfos = 0;
		score = 0;
		lastPoint = 0;
		rocketx = carx;
		rockety = 0;
		shooted = false;
		gameStart = false;
	}


	if (key == GLUT_KEY_F1) {
		gameStart = !gameStart;
	}

	if (key == GLUT_KEY_RIGHT && carx < 430 && gameStart && !gameStop)
	{
		carx += 10;
		rev = 1;
	}
	if (key == GLUT_KEY_LEFT && carx > -530 && gameStart && !gameStop)
	{
		carx -= 10;
		rev = -1;
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
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
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


void rocketTimer() {
	if (shooted)
		rockety += 15;

	if (rockety % 15 == 0)
		rocketFire = !rocketFire;
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	
	
	if (gameStart && !gameStop)
	{
		int m;
		timer-=3;
		if (timer <= 0)
			gameStop = true;
		rocketTimer();

	//	r = (r + 10) % 255;
	//	g = (g + 10) % 255;
	//	b = (b + 10) % 255;

		for (int i = 0; i < 5; i++)
			if (!ufo[i].initialized)
			{	
				ufo[i].y = rand() % 450;
				ufo[i].initialized = true;
				
			}
			


		for (int i = 0; i < 5; i++)
			if (ufo[i].initialized) {			
				ufo[i].x += 10;

				if (ufo[i].x > -670 && !ufo[i].countChecker)
				{
					totalUfos++;
					ufo[i].countChecker = true;
				}
			}

		for (int i = 0; i < 5; i++) 
			if (ufo[i].x > 430)
			{
				ufo[i].x = -675 - i * 400;
				ufo[i].initialized = false;
				ufo[i].countChecker = false;
			}
		

		for (int i = 0; i < 5; i++) {
		
			if(ufo[i].x == rocketx && ufo[i].x == rocketx && ufo[i].y + 70 >= rockety - 210 && ufo[i].y - 70 <= rockety - 210){
				ufo[i].x = -675 - i * 400;
				ufo[i].initialized = false;
				ufo[i].countChecker = false;
				score += 5;
				burstUfos++;
				lastPoint = 5;
			}

			else if (ufo[i].x + 20 >= rocketx && ufo[i].x - 20 <= rocketx && ufo[i].y >= rockety - 210 && ufo[i].y - 70 <= rockety - 210)
			{
				ufo[i].x = -675 - i * 400;
  				ufo[i].initialized = false;
				ufo[i].countChecker = false;
				score += 3;
				burstUfos++;
				lastPoint = 3;
			}
			else if (ufo[i].x + 30 >= rocketx && ufo[i].x - 30 <= rocketx && ufo[i].y + 70 >= rockety - 210 && ufo[i].y - 70 <= rockety - 210)
			{
				ufo[i].x = -675 - i * 400;
				ufo[i].initialized = false;
				ufo[i].countChecker = false;
				score += 2;
				burstUfos++;
				lastPoint = 2;
			}
			
			else if (ufo[i].x + 65 >= rocketx && ufo[i].x - 65
				<= rocketx && ufo[i].y + 70 >= rockety - 210 && ufo[i].y - 70 <= rockety - 210)
			{
				ufo[i].x = -675 - i * 400;
				ufo[i].initialized = false;
				ufo[i].countChecker = false;
				score += 1;
				burstUfos++;
				lastPoint = 1;
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

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Blowing up baloons");

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
	
	
	srand(time(NULL));
	
	for (int i = 0; i < 5; i++) {
		ufo[i].x = -675 - i * 300;
		ufo[i].initialized = false;
	}


#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}