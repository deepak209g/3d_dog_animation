/* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above notice and this permission notice shall be included in all copies
* or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
/* File for "Basic Shapes" lesson of the OpenGL tutorial on
* www.videotutorialsrock.com
*/



#include <iostream>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>
#include <math.h>
#include "imageloader.h"
#define PI 3.14159265
using namespace std;

typedef struct vertex{
	float x;
	float y;
	float z;
}vertex;

typedef struct DOGE{
	float x;
	float y;
	float z;
	int dir;
	float r;
	float g;
	float b;
}DOGE;

GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]
vertex vertices[8];
int faces[6][4];
int theta = 25;
int ltheta;
int FORWARD = 0;
int BACKWARD = 1;
int FRBL = FORWARD;
int FLBR = BACKWARD;
GLuint _textureId1, _textureId2, _textureId3, _textureId4;
bool is_first_time = true;
int mx, my;
double cMotion = 0.5;
int SOLID = 0;
int WIRE = 1;
int frameType = SOLID;


float fa[60][2];
float ba[60][3];
float dfa[12][2] = {
	{ 30, 90 },
	{ 32, 75 },
	{ 20, 60 },
	{ 0, 60 },
	{ -40, 10 },
	{ -60, -15 },
	{ -70, -10 },
	{ -80, -35 },
	{ -90, -5 },
	{ -60, 65 },
	{ -30, 75 },
	{ 10, 85 }
};

float tail[12][10] = {
	{ 0, 1, 2, 1, 0, -1, -2, -3, -4, -3 },
	{ 0, 1, 2, 1, 0, -1, -2, -3, -2, -1 },
	{ 0, 1, 2, 1, 0, -1, -2, -3, -2, -1 },
	{ 0, 1, 2, 1, 0, -1, -2, -3, -3, -2 },
	{ 0, -1, -2, -3, -4, -4, -4, -4, -3, -2 },
	{ 0, -1, -2, -3, -4, -5, -5, -6, -6, -6 },
	{ 0, -1, -2, -3, -4, -5, -6, -7, -8, -9 },
	{ 0, -1, -2, -3, -4, -5, -5, -6, -7, -8 },
	{ 0, 0, -1, -1, -2, -3, -3, -4, -5, -6 },
	{ 0, 1, 0, -1, -2, -3, -4, -5, -5, -5 },
	{ 0, 0, 1, 0, -1, -1, -2, -3, -4, -5 },
	{ 0, 1, 0, -1, -1, -2, -3, -3, -4, -5 }
};

float dba[12][3] = {
	{ -45, -105, -30 },
	{ -10, -110, -15 },
	{ -5, -120, -30 },
	{ 20, -90, -5 },
	{ 35, -50, 35 },
	{ 50, 0, 50 },
	{ 45, -5, 45 },
	{ 40, -5, 40 },
	{ 10, -70, 15 },
	{ -30, -80, -25 },
	{ -40, -90, -30 },
	{ -60, -90, -45 }
};


int fri = 10;
int fli = 0;
int bri = 10;
int bli = 0;
int frames = 60;
int steps = 4;

float feetHeight = 3.0;


int rotae = 0;
float cameraATx, cameraATz;
float cameraATy = 0;
int cameraTheta = 0;
float cameraX = 0, cameraY = 8, cameraZ = 30;


DOGE dog1;
DOGE dog2;

DOGE moveDog(DOGE dog,float step){
	dog.x += step*cos(dog.dir * PI / 180);
	dog.z -= step*sin(dog.dir * PI / 180);
	return dog;
}

void doge(DOGE dog){
	glPushMatrix();
	glColor3f(dog.r, dog.g, dog.b);
	glTranslatef(dog.x, dog.y, dog.z);
	glRotatef(dog.dir, 0, 1, 0);
	// tail
	float tailer = -2.5;
	float scale = 0.3;
	int tidx = 0;
	for (int t = 0; t < 7; t++){
		glPushMatrix();
		glTranslatef(tailer, tail[fri / 12][t] / 8, 0);
		if (frameType == SOLID)
			glutSolidCube(scale);
		else
			glutWireCube(scale);
		glPopMatrix();
		scale += 0.03;
		tailer -= (scale + 0.03);
	}
	for (int t = 0; t < 3; t++){
		glPushMatrix();
		glTranslatef(tailer, tail[fri / 12][t + 7] / 8, 0);
		if (frameType == SOLID)
			glutSolidCube(scale);
		else
			glutWireCube(scale);
		glPopMatrix();
		scale -= 0.1;
		tailer -= (scale + 0.03);
	}
	// torso
	glPushMatrix();
	glTranslatef(-2.5, 0, 0);
	glScalef(0.3, 0.8, 0.8);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.2, 0, 0);
	glScalef(0.3, 1.2, 1);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.8, 0, 0);
	glScalef(0.5, 1.3, 1.1);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 0, 0);
	glScalef(0.5, 1.4, 1.2);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.6, 0, 0);
	glScalef(0.5, 1.5, 1.3);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0.5, 1.6, 1.4);
	if (frameType == SOLID)
		glutSolidCube(1);
	else glutWireCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.6, 0, 0);
	glScalef(0.5, 1.7, 1.5);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.2, 0, 0);
	glScalef(0.5, 1.6, 1.4);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();


	glPushMatrix();
	glTranslatef(1.8, 0, 0);
	glScalef(0.4, 1.5, 1.3);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.3, 0, 0);
	glScalef(0.3, 1.4, 1.2);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.65, 0.2, 0);
	glScalef(0.2, 1, 1);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.9, 0.2, 0);
	glScalef(0.1, 0.9, 0.9);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();



	// Head Of the animal here


	//face1
	glPushMatrix();
	glTranslatef(3.0, 0.2, 0);
	glScalef(0.7, 1.5, 1.5);
	if (frameType == SOLID){
		glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(3.35, 0.70, 0.38);
		glutSolidSphere(0.2, 22, 22);
		glTranslatef(0, 0, -0.6);
		glutSolidSphere(0.2, 22, 22);
		glPopMatrix();
	}
	else{
		glutWireCube(1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(3.35, 0.70, 0.38);
		glutWireSphere(0.2, 5, 5);
		glTranslatef(0, 0, -0.6);
		glutWireSphere(0.2, 5, 5);
		glPopMatrix();
	}


	//glPopMatrix();


	// face2
	glPushMatrix();
	glTranslatef(3.5, 0, 0);
	glScalef(0.3, 0.8, 0.8);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();

	// face3
	glPushMatrix();
	glTranslatef(3.9, 0, 0);
	glScalef(0.75, 0.25, 0.5);

	glPushMatrix();
	glTranslatef(0, 0.2, 0);
	glRotatef(20, 0, 0, 1);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, -0.8, 0);
	glRotatef(-20, 0, 0, 1);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);
	glPopMatrix();
	glPopMatrix();

	//front right
	glPushMatrix();
	glTranslatef(2, 0, 1);
	//upper
	glPushMatrix();
	glRotatef(fa[fri][0], 0, 0, 1);
	glScalef(0.5, feetHeight / 1.75, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//lower
	glPushMatrix();
	float y = feetHeight * 1 / 1.75 * cos(fa[fri][0] * PI / 180);
	float x = feetHeight * 1 / 1.75 * sin(fa[fri][0] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(fa[fri][1], 0, 0, 1);
	glScalef(0.5, feetHeight / 2, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//fri = (fri + 1) % frames;
	glPopMatrix();



	//front left
	glPushMatrix();
	glTranslatef(2, 0, -1);
	//upper
	glPushMatrix();
	glRotatef(fa[fli][0], 0, 0, 1);
	glScalef(0.5, feetHeight / 1.75, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//lower
	glPushMatrix();
	y = feetHeight * 1 / 1.75 * cos(fa[fli][0] * PI / 180);
	x = feetHeight * 1 / 1.75 * sin(fa[fli][0] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(fa[fli][1], 0, 0, 1);
	glScalef(0.5, feetHeight / 2, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//fli = (fli + 1) % frames;
	glPopMatrix();

	//back right
	glPushMatrix();
	glTranslatef(-2, 0, 1);
	//upper
	glPushMatrix();
	glRotatef(ba[bri][0], 0, 0, 1);
	glScalef(0.5, feetHeight / 2, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//middle
	glPushMatrix();
	y = feetHeight * 1 / 2 * cos(ba[bri][0] * PI / 180);
	x = feetHeight * 1 / 2 * sin(ba[bri][0] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(ba[bri][1], 0, 0, 1);
	glScalef(0.5, feetHeight / 3, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//lower
	glPushMatrix();
	y = y + feetHeight * 1 / 3 * cos(ba[bri][1] * PI / 180);
	x = x + feetHeight * 1 / 3 * sin(ba[bri][1] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(ba[bri][2], 0, 0, 1);
	glScalef(0.5, feetHeight / 3, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//bri = (bri + 1) % frames;
	glPopMatrix();


	//back left
	glPushMatrix();
	glTranslatef(-2, 0, -1);
	//upper
	glPushMatrix();
	glRotatef(ba[bli][0], 0, 0, 1);
	glScalef(0.5, feetHeight / 2, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//middle
	glPushMatrix();
	y = feetHeight * 1 / 2 * cos(ba[bli][0] * PI / 180);
	x = feetHeight * 1 / 2 * sin(ba[bli][0] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(ba[bli][1], 0, 0, 1);
	glScalef(0.5, feetHeight / 3, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//lower
	glPushMatrix();
	y = y + feetHeight * 1 / 3 * cos(ba[bli][1] * PI / 180);
	x = x + feetHeight * 1 / 3 * sin(ba[bli][1] * PI / 180);
	glTranslatef(x, -y, 0);
	glRotatef(ba[bli][2], 0, 0, 1);
	glScalef(0.5, feetHeight / 3, 0.5);
	glTranslatef(0, -0.5, 0);
	if (frameType == SOLID)
		glutSolidCube(1);
	else
		glutWireCube(1);;
	glPopMatrix();
	//bli = (bli + 1) % frames;
	glPopMatrix();
	glPopMatrix();
}

void lamp(int x, int y, int z, float r, float g, float b){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(0, 0.1, 0.1);
	if (frameType == SOLID){
		glPushMatrix();
		glScalef(0.8, 9, 0.8);
		glTranslatef(0, 0.5, 0);
		glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0, 8.5, 0);
		glColor3f(r, g, b);
		glutSolidSphere(1.2, 17, 17);
		glPopMatrix();
	}
	else{
		glPushMatrix();
		glScalef(0.8, 9, 0.8);
		glTranslatef(0, 0.5, 0);
		glutWireCube(1);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(0, 8.5, 0);
		glColor3f(r, g, b);
		glutWireSphere(1.2, 17, 17);
		glPopMatrix();
	}
	
	glPopMatrix();

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

	glLoadIdentity();


	
	//gluLookAt(cameraX, cameraY, cameraZ, cameraATx, cameraATy, cameraATz, 0.0, 1.0, 0.0);//Eye
	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat directedLightPos[] = { -10.0f, 15.0f, 20.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);


	glRotatef(cameraTheta, 0, 1, 0);
	glTranslatef(-cameraX, -cameraY, -cameraZ);

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 2.0, 2.0, 2.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0, 6, 10, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	//lamp
	lamp(0, -4, 10, 1, 1, 0);
	lamp(-20, -4, -10, 1, 1, 0);
	lamp(20, -4, -10, 1, 1, 0);
	
	//glutSolidCube(2);
	//glTranslatef(0, 0, -15);
	//glRotatef(angleCube, 1.0f, 1.0f, 1.0f);  // Rotate about (1,1,1)-axis [NEW]

	//set up background
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId3);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);

	glVertex3f(-700.5f, -3.5f, 120.5f);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(700.5f, -3.5f, 120.5f);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(700.5f, -3.5f, -25.5f);
	glTexCoord2f(0.0f, 5.0f);
	glVertex3f(-700.5f, -3.5f, -25.5f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, _textureId4);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	/*glBegin(GL_QUADS);

	glNormal3f(0.0, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-4.5f, 3.5f, 2.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.5f, 3.5f, 2.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(4.5f, 3.5f, -15.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.5f, 3.5f, -15.5f);

	glEnd();
	*/
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	//Back
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-700.5f, -4.5f, -15.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-700.5f, 100.5f, -15.5f);
	glTexCoord2f(5.0f, 1.0f);
	glVertex3f(700.5f, 100.5f, -15.5f);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(700.5f, -4.5f, -15.5f);

	glEnd();
	/*
	//Left
	glBindTexture(GL_TEXTURE_2D, _textureId2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/
	/*
	//glColor3f(1.0f, 0.7f, 0.3f);
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-700.5f, -3.5f, 2.5f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-700.5f, -3.5f, -15.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-700.5f, 3.5f, -15.5f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-700.5f, 3.5f, 2.5f);

	glEnd();
	
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(700.5f, -3.5f, 2.5f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(700.5f, -3.5f, -15.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(700.5f, 3.5f, -15.5f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(700.5f, 3.5f, 2.5f);

	glEnd();
	*/


	//------------

	doge(dog1);
	doge(dog2);
	glutSwapBuffers();
	anglePyramid += 0.2f;
	angleCube -= 0.15f;

}


/* Called back when timer expired [NEW] */
void timer(int value) {
	glutPostRedisplay();      // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y)
{
	cout << key << endl;
	switch (key) {
	case 27:
		exit(0);
		break;
	case 110:
		glutPostRedisplay();
		break;
	case 'w':
		//w
		if (frameType == SOLID)
			frameType = WIRE;
		else
			frameType = SOLID;
		glutPostRedisplay();
		break;
	case 'f':
		fli = (fli + 1) % frames;
		fri = (fri + 1) % frames;
		bli = (bli + 1) % frames;
		bri = (bri + 1) % frames;
		dog1 = moveDog(dog1, 0.2);
		dog2 = moveDog(dog2, 0.2);
		glutPostRedisplay();
		break;

	case 'b':
		if (fli == 0){
			fli = frames - 1;
		}
		else{
			fli--;
		}
		if (fri == 0){
			fri = frames - 1;
		}
		else{
			fri--;
		}
		if (bli == 0){
			bli = frames - 1;
		}
		else{
			bli--;
		}
		if (bri == 0){
			bri = frames - 1;
		}
		else{
			bri--;
		}
		dog1 = moveDog(dog1, -0.2);
		dog2 = moveDog(dog2, -0.2);
		glutPostRedisplay();
		break;
	
	}
}


void mouseMotion(int x, int y){
	if (is_first_time){
		mx = x;
		my = y;
		is_first_time = false;
	}
	else{
		int diffx = x - mx;
		int diffy = y - my;
		cameraTheta += diffx;
		cameraTheta = cameraTheta % 360;
		printf("cameraTheta %d \n", cameraTheta);
		glutPostRedisplay();
		mx = x;
		my = y;
	}
}


GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image->width, image->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image->pixels);
	return textureId;
}


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	// initialise values of fa and ba
	int i;
	int counter = 0;
	for (i = 0; i < 12; i++){
		int j;
		fa[counter][0] = dfa[i][0];
		fa[counter][1] = dfa[i][1];
		ba[counter][0] = dba[i][0];
		ba[counter][1] = dba[i][1];
		ba[counter][2] = dba[i][2];
		counter++;
		for (j = 1; j <= steps; j++){
			fa[counter][0] = dfa[i][0] + j / (steps + 1) * (dfa[(i + 1) % 12][0] - dfa[i][0]);
			fa[counter][1] = dfa[i][1] + j / (steps + 1) * (dfa[(i + 1) % 12][1] - dfa[i][1]);
			ba[counter][0] = dba[i][0] + j / (steps + 1) * (dba[(i + 1) % 12][0] - dba[i][0]);
			ba[counter][1] = dba[i][1] + j / (steps + 1) * (dba[(i + 1) % 12][1] - dba[i][1]);
			ba[counter][2] = dba[i][2] + j / (steps + 1) * (dba[(i + 1) % 12][2] - dba[i][2]);
			counter++;
		}
	}
	
	dog1.x = -25;
	dog1.y = 0;
	dog1.z = -3;
	dog1.dir = 0;
	dog1.r = 0.5;
	dog1.g = 0.4;
	dog1.b = 0.3;

	dog2.x = 25;
	dog2.y = 0;
	dog2.z = 3;
	dog2.dir = 180;
	dog2.r = 0.5;
	dog2.g = 0.4;
	dog2.b = 0.3;

	Image* image1 = loadBMP("back.bmp");
	_textureId1 = loadTexture(image1);

	Image* image2 = loadBMP("bottom2.bmp");
	_textureId2 = loadTexture(image2);

	Image* image3 = loadBMP("bottom2.bmp");
	_textureId3 = loadTexture(image3);

	Image* image4 = loadBMP("bottom2.bmp");
	_textureId4 = loadTexture(image4);

	delete image1, image2, image3, image4;
}

void keySpecial(int key, int x, int y) {
	switch (key){
	case GLUT_KEY_UP:
		//w
		cameraZ -= cMotion*cos(cameraTheta*PI / 180);
		cameraX += cMotion*sin(cameraTheta * PI / 180);
		//cameraZ -= cMotion;
		printf("cameraZ : %f, cameraX : %f", cameraZ, cameraX);
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		//a
		cameraZ -= cMotion*sin(cameraTheta * PI / 180);
		cameraX -= cMotion*cos(cameraTheta*PI / 180);
		printf("cameraZ : %f, cameraX : %f", cameraZ, cameraX);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		//s
		cameraZ += cMotion*cos(cameraTheta*PI / 180);
		cameraX -= cMotion*sin(cameraTheta * PI / 180);
		printf("cameraZ : %f, cameraX : %f", cameraZ, cameraX);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		//d
		cameraZ += cMotion*sin(cameraTheta * PI / 180);
		cameraX += cMotion*cos(cameraTheta*PI / 180);
		printf("cameraZ : %f, cameraX : %f", cameraZ, cameraX);
		glutPostRedisplay();
		break;
	}
}

int main(int argc, char **argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Computer Graphics Assignment");
	initRendering();
	//loadGeometry("Cuboid.txt", vertices, faces);
	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keySpecial);
	//glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}