
#include <iostream>
using namespace std;
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <irrKlang.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
bool side = true;
bool front = false;
bool top = true;
bool sideH = false;
bool frontH = false;
bool topH = false;

bool win = false;
bool lose = false;
bool play = true;
int counter = 0;
int timeCounter = 1;
double posX = 0.5;
double posZ = 0.5;
int movDirection = 0;
double posGiftX1 = 0.1;
double posGiftZ1 = 0.7;
double posGiftX2 = 0.9;
double posGiftZ2 = 0.5;
double posGiftX3 = 0.5;
double posGiftZ3 = 0.9;

double posShape1AX = 0.1;
double posShape1AZ = 0.5;
double posShape1BX = 0.5;
double posShape1BZ = 0.3;


bool collision1 = false;
bool collision2 = false;
bool collision3 = false;
bool animFenceB = false;
double animFence = 0;
bool animTreeB = false;
double animTree = 0;
bool animSnowManB = false;
double animSnowMan = 0;
bool animShape1B = false;
bool shape1AXPlus = true;
bool shape1AZPlus = true;
bool shape1BXPlus = true;
bool shape1BZPlus = true;

double animShape1AAX = 0;
double animShape1BBX = 0;

double animShape1AAZ = 0;
double animShape1BBZ = 0;
bool animShape2B = false;
double animShape2 = 0;
bool animShape3B = false;
double animShape3 = 0;
bool shape3Plus = true;
bool animPresentB = false;
double animPresent = 0;



class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};


class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.5, float eyeY = 1, float eyeZ = 2.2, float centerX = 0.5, float centerY = 0.5, float centerZ = 0, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}


	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;
void print(int x, int y, char* string) {
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}
void drawGround(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawCylinder(double baseRadius, double topRadius, double height ) {
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, baseRadius, topRadius, height, 50, 50);
}
void fenceBlock(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick*10);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawFenceLeft( double y) {
	
	glPushMatrix();
	glTranslated(0, y+0.225, 0.05);
	glRotated(90, 1, 0, 0);
	glColor3f(0.3, 0.2, 0.1);
	fenceBlock(0.01, 0.9);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0,y+0, 0.05);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, y+0, 0.23);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, y+0, 0.41);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, y+0, 0.59);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0, y+0,0.77);
	fenceBlock(0.01, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0,y+ 0, 0.95);
	fenceBlock(0.01, 0.5);
	glPopMatrix();
}

void drawFenceTop(double y) {


	glPushMatrix();
	glTranslated(0.05, y+0.25, 0.05);
	glRotated(-90, 1, 0, 0);
	glRotated(-90, 0, 0, 1);
	glColor3f(0.3, 0.2, 0.1);
	fenceBlock(0.01, 0.9);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.05, y+0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.23,y+ 0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.41,y+ 0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.59,y+ 0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.77,y+ 0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.95,y+ 0, 0);
	glRotated(-90, 0, 1, 0);
	fenceBlock(0.01, 0.5);
	glPopMatrix();



}

void drawFenceRight(double y) {

	glPushMatrix();
	glTranslated(1, y+0.225, 0.05);
	glRotated(90, 1, 0, 0);
	glColor3f(0.3, 0.2, 0.1);
	fenceBlock(0.01, 0.9);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1, y+0, 0.05);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1, y+0, 0.23);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1, y+0, 0.41);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1, y+0, 0.59);
	fenceBlock(0.01, 0.5);
	glPopMatrix();


	glPushMatrix();
	glTranslated(1, y+0, 0.77);
	fenceBlock(0.01, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1, y+0, 0.95);
	fenceBlock(0.01, 0.5);
	glPopMatrix();
}

void drawPlayer(double x, double z,int dir ) {

	glPushMatrix();
	glTranslated(x, 0, z);
	glRotated(90 * dir, 0, 1, 0);
	glTranslated(-x, 0, -z);
	
	//foot
	glPushMatrix();
	glTranslated(x, 0, z);
	glScaled(0.1,.05 , 0.05);
	glColor3d(0, 0, 0);
	glutSolidCube(1);
	glPopMatrix();

	//legs
	glPushMatrix();
	glTranslated(x-0.03, 0.2, z);
	glRotated(90, 1, 0, 0);
	glColor3d(1, 0, 0);
	drawCylinder(0.01, 0.01, 0.2);
	glPopMatrix();


	glPushMatrix();
	glTranslated(x+0.03, 0.2, z);
	glRotated(90, 1, 0, 0);
	glColor3d(1, 0, 0);
	drawCylinder(0.01, 0.01, 0.2);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslated(x, 0.5, z);
	glRotated(90, 1, 0, 0);
	glColor3d(1, 0, 0);
	drawCylinder(0, 0.08, 0.4);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, 0.105, z);
	glScaled(1, .5, 1);
	glColor3d(1, 1, 1);
	glutSolidSphere(0.0807, 50, 50);
	glPopMatrix();

	//belt
	glPushMatrix();
	glTranslated(x, 0.22, z);
	glScaled(1, .39, 1);
	glColor3f(0.6, 0.3, 0.1);
	glutSolidSphere(0.059, 50, 50);
	glPopMatrix();

	//neck
	glPushMatrix();
	glTranslated(x, 0.35, z);
	glScaled(1, .45, 1);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.045, 50, 50);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslated(x, 0.39, z);
	glColor3f(0.9,0.8,0.6);
	glutSolidSphere(0.04, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x-0.018, 0.39, z+0.036);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.004, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.018, 0.39, z + 0.036);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.004, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x , 0.38, z + 0.036);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.004, 50, 50);
	glPopMatrix();


	//Arms 
	glPushMatrix();
	glTranslated(x, 0.35, z);
	glRotated(35, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glColor3d(1, 0, 0);
	drawCylinder(0.015, 0.015, 0.15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x+.083, 0.23, z);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.0144, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, 0.35, z);
	glRotated(-35, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glColor3d(1, 0, 0);
	drawCylinder(0.015, 0.015, 0.15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x-.083, 0.23, z);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.0144, 50, 50);
	glPopMatrix();

	//capuche
	glPushMatrix();
	glTranslated(x, 0.51, z);
	glRotated(90, 1, 0, 0);
	glColor3f(1, 0, 0);
	drawCylinder(0, 0.04, 0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, 0.413, z);
	glScaled(1, 0.5, 1);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.041, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, 0.506, z);
	glColor3f(0.9, 0.8, 0.6);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPopMatrix();


}

void drawTree(double x, double y, double z) {

	glPushMatrix();
	glTranslated(x, y+0.1, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0.3 , 0.2 , 0.1);
	drawCylinder(0.05, 0.05, 0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y+0.7, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0 , 0.4 , 0.1);
	drawCylinder(0, 0.15, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x-0.05, y + 0.31, z+0.13);
	glColor3f(0.7 , 0.1 , 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.05, y + 0.35, z + 0.125);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x - 0.07, y + 0.39, z + 0.12);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();
    
	glPushMatrix();
	glTranslated(x - 0.03, y + 0.41, z + 0.115);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.03, y + 0.43, z + 0.11);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.09, y + 0.45, z + 0.105);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x - 0.05, y + 0.52, z + 0.123);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x - 0.02, y + 0.56, z + 0.118);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.08, y + 0.6, z + 0.11);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x , y + 0.64, z + 0.102);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.68, z + 0.06);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x-0.03, y + 0.74, z -0.02);
	glColor3f(0.7, 0.1, 0.2);
	glutSolidSphere(0.01, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y+0.9, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0, 0.4, 0.1);
	drawCylinder(0, 0.14, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y+1, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0, 0.4, 0.1);
	drawCylinder(0, 0.13, 0.5);
	glPopMatrix();

	
}

void drawShape1(double x, double y, double z) {
	glPushMatrix();
	glTranslated(x, y + 0.22, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinder(0.03, 0.03, 0.22);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.111, z);
	glScaled(1, 0.8, 1);
	glColor3f(0.976, 0.952, 0.541);
	glutSolidSphere(0.1, 50, 50);
	glPopMatrix();

}

void drawShape2(double x, double y, double z) {
	glPushMatrix();
	glTranslated(x, y + 0.25, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0, 0, 0);
	drawCylinder(0.02, 0.02, 0.25);
	glPopMatrix();
	glPushMatrix();
	glTranslated(x, y + 0.233, z);
	glRotated(-270, 1, 0, 0);
	glColor3f(0.956, 0.576, 0.023);
	drawCylinder(0.03, 0.03, 0.2);
	glPopMatrix();

}

void drawShape3(double x, double y, double z) {
	glPushMatrix();
	glTranslated(x+0.08, y+0.04, z);
	glColor3f(0.243, 0.149, 0);
	glutSolidCube(0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.08, y + 0.3, z);
	glColor3f(0.643, 0.062, 0.015);
	glScaled(0.3, 5, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(x-0.08, y + 0.04, z);
	glColor3f(0.243, 0.149, 0);
	glutSolidCube(0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x - 0.08, y + 0.3, z);
	glColor3f(0.643, 0.062, 0.015);
	glScaled(0.3, 5, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x , y + 0.4, z);
	glRotated(90, 0, 0, 1);
	glColor3f(0.643, 0.062, 0.015);
	glScaled(0.5, 3, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.45, z);
	glColor3f(0.643, 0.062, 0.015);
	glScaled(0.5, 1, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.5, z);
	glRotated(90, 0, 0, 1);
	glColor3f(0.643, 0.062, 0.015);
	glScaled(0.5, 4, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.55, z);
	glRotated(90, 0, 0, 1);
	glColor3f(0, 0, 0);
	glScaled(0.5, 4.5, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

}

void drawSnowman(double x, double y, double z) {
	glPushMatrix();
	glTranslated(x, 0, z);
	glRotated(-45 + y, 0, 1, 0);
	glTranslated(-x, 0, -z);

	glPushMatrix();
	glTranslated(x , 0.1 , z);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.09, 50, 50);
	glPopMatrix();



	glPushMatrix();
	glTranslated(x, 0.23, z);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.067, 50, 50);
	glPopMatrix();


	//Buttons
	glPushMatrix();
	glTranslated(x ,  0.23+ 0.01, z + 0.06);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.009, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, 0.23 -0.01, z + 0.06);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.009, 50, 50);
	glPopMatrix();

	//Neck
	glPushMatrix();
	glTranslated(x, 0.295, z);
	glScaled(1, .45, 1);
	glColor3f(1, 0, 0);
	glutSolidSphere(0.045, 50, 50);
	glPopMatrix();

	//Head 

	glPushMatrix();
	glTranslated(x, 0.337, z);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.04, 50, 50);
	glPopMatrix();

	//Eyes 

	glPushMatrix();
	glTranslated(x-0.02,  0.35, z+ 0.04);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.004, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x + 0.02,  0.35, z + 0.04);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.004, 50, 50);
	glPopMatrix();

	//Nose

	glPushMatrix();
	glTranslated(x,  0.337, z+0.079);
	glRotated(180, 1, 0, 0);
	glColor3f(0.929, 0.576, 0.231);
	drawCylinder(0.001, 0.01, 0.04);
	glPopMatrix();

	//Capuche 0.413

	glPushMatrix();
	glTranslated(x,  0.45, z);
	glRotated(90, 1, 0, 0);
	glColor3f(1, 0, 0);
	drawCylinder(0, 0.035, 0.1);
	glPopMatrix();


	glPushMatrix();
	glTranslated(x, 0.45, z);
	glColor3f(0.9, 0.8, 0.6);
	glutSolidSphere(0.01, 50, 50); 
	glPopMatrix();

	glPopMatrix();


}

void drawPresent(double x, double y, double z) {
	glPushMatrix();
	glTranslated(x , y + 0.07, z);
	glColor3f(0.913, 0.101, 0.027);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.07, z);
	glScaled(0.1, 1.05, 1.05);
	glColor3f(0, 0, 0);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(x, y + 0.07, z);
	glScaled(1.05, 1.05, 0.1);
	glColor3f(0, 0, 0);
	glutSolidCube(0.1);
	glPopMatrix();

}


void collisions(double xPlayer, double zPlayer, double xPresent1, double zPresent1, double xPresent2, double zPresent2, double xPresent3, double zPresent3) {
	if (xPlayer<xPresent1 + 0.07 && xPlayer > xPresent1 - 0.07 && zPlayer<zPresent1 + 0.07 && zPlayer > zPresent1 - 0.07 && !collision1) {
		collision1 = true;
  }  
	else if (xPlayer<xPresent2 + 0.07 && xPlayer > xPresent2 - 0.07 && zPlayer<zPresent2 + 0.07 && zPlayer > zPresent2 - 0.07 && !collision2) {
		collision2 = true;
	}
	else if (xPlayer<xPresent3 + 0.07 && xPlayer > xPresent3 - 0.07 && zPlayer<zPresent3 + 0.07 && zPlayer > zPresent3 - 0.07 && !collision3) {
		collision3 = true;
	}

}
void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1920 / 1080, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}


void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (win) {
		
		PlaySound(NULL, 0, 0);
			PlaySound(TEXT("win.wav"), NULL, SND_FILENAME);
	
		exit(EXIT_SUCCESS);
	}
	else if (animPresent >= 1.6) {
		
		PlaySound(NULL, 0, 0);
			PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME);
		
		exit(EXIT_SUCCESS);
	}
	
		glPushMatrix();
		glColor3d(0.3, 0.3, 0.3);
		drawGround(0.02);
		glPopMatrix();

		glPushMatrix();
		drawFenceLeft(animFence);
		drawFenceTop(animFence);
		drawFenceRight(animFence);
		glPopMatrix();

		glPushMatrix();
		drawPlayer(posX, posZ, movDirection);
		glPopMatrix();

		glPushMatrix();
		drawTree(0.2, animTree, 0.25 + animTree);
		drawTree(0.8, animTree, 0.25 + animTree);
		glPopMatrix();

		glPushMatrix();
		drawShape1(posShape1AX + animShape1AAX, 0, posShape1AZ + animShape1AAZ);
		drawShape1(posShape1BX + animShape1BBX, 0, posShape1BZ + animShape1BBZ);
		glPopMatrix();

		glPushMatrix();
		drawShape2(.1, animShape2, 0.9);
		glPopMatrix();

		glPushMatrix();
		drawShape3(0.5 + animShape3, 0, 0.09);
		glPopMatrix();


		glPushMatrix();
		drawSnowman(0.9, animSnowMan, 0.9);
		glPopMatrix();


		collisions(posX, posZ, posGiftX1, posGiftZ1, posGiftX2, posGiftZ2, posGiftX3, posGiftZ3);
		glPushMatrix();
		if (!collision1)
			drawPresent(posGiftX1, animPresent, posGiftZ1);
		if (!collision2)
			drawPresent(posGiftX2, animPresent, posGiftZ2);
		if (!collision3)
			drawPresent(posGiftX3, animPresent, posGiftZ3);
		glPopMatrix();
		if (collision1 && collision2 && collision3)
			win = true;
	
	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case 'i':
		if(posZ>0.03)
		posZ -= 0.01;
		movDirection = 2;
		break;
	case 'k' :
		if (posZ < 0.97)
		posZ += 0.01;
		movDirection = 0;
		break;
	case 'j' :
		if (posX > 0.05)
		posX -= 0.01;
		movDirection = 3;
		break;
	case 'l':
		if (posX < 0.95)
		posX += 0.01;
		movDirection = 1;
		break;
	case 'z':
		if (!animFenceB)
			animFenceB = true;
		else {
			animFenceB = false;
			animFence = 0;
		}
		break;
	case 'x': 
		if (!animTreeB)
			animTreeB = true;
		else {
			animTreeB = false;
			animTree = 0;
		}
		break;

	case 'c':
		if (!animShape1B)
			animShape1B = true;
		else {
			animShape1B = false;
			animShape1AAX = 0;
			animShape1AAZ = 0;
			animShape1BBX = 0;
			animShape1BBZ = 0;
		}
		break;

	case 'v':
		if (!animShape2B)
			animShape2B = true;
		else {
			animShape2B = false;
			animShape2 = 0;

		}
		break;
	case 'b':
		if (!animShape3B)
			animShape3B = true;
		else {
			animShape3B = false;
			animShape3 = 0;

		}
		break;
	case 'n':
		if (!animSnowManB)
			animSnowManB = true;
		else {
			animSnowManB = false;
			animSnowMan = 0;

		}
		break;

		break;
	case 'f': //front
		if (front) {
			if (sideH) {
				camera.rotateY(-90);
				camera.moveZ(-1.7);
				camera.moveX(2);
				sideH = false;
				side = true;
			}
			if (topH) {
				camera.rotateX(90);
				camera.moveZ(-1.7);
				camera.moveY(-1);
				topH = false;
				top = true;
			}

		}

		 
		break;
	case 'g': //side
		if (side) {
			if (topH) {
				camera.rotateX(90);
				camera.moveZ(-1.7);
				camera.moveY(-1);
				topH = false;
				top = true;
			}
			
			camera.moveX(-2);
			camera.moveZ(1.7);
			camera.rotateY(90);
			side = false;
			sideH = true;
			front = true;
			top = true;
		}
		break;
	case 'h': //side
		if (top) {
			if (sideH) {
				camera.rotateY(-90);
				camera.moveZ(-1.7);
				camera.moveX(2);
				sideH = false;
				side = true;
			}
			camera.moveY(1);
			camera.moveZ(1.7);
			camera.rotateX(-90);
			top = false;
			topH = true;
			front = true;
			side = true;
		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	std::cout << animPresent<<'/ n';
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}
void Time(int x) {
std:cout << animPresent << "\n";
	if (!lose) {
	timeCounter++;
	if (timeCounter % 1000 == 0) {
		counter++;
		timeCounter = 1;
	}
	if (counter == 3)
		lose = true;
	if (animFenceB)
		animFence += 0.005;
	if (animTreeB)
		animTree += 0.005;
	if (animShape1B) {
		if ((animShape1AAX + posShape1AX) < 0.95 && shape1AXPlus) {
			animShape1AAX += 0.005;
		}
		else {
			shape1AXPlus = false;
			animShape1AAX -= 0.005;
			if ((animShape1AAX + posShape1AX) <= 0.1)
				shape1AXPlus = true;

		}
		if ((animShape1AAZ + posShape1AZ) < 0.95 && shape1AZPlus) {
			animShape1AAZ += 0.005;
		}
		else {
			shape1AZPlus = false;
			animShape1AAZ -= 0.005;
			if ((animShape1AAZ + posShape1AZ) <= 0.1)
				shape1AZPlus = true;

		}

		if ((animShape1BBX + posShape1BX) < 0.95 && shape1BXPlus) {
			animShape1BBX += 0.005;
		}
		else {
			shape1BXPlus = false;
			animShape1BBX -= 0.005;
			if ((animShape1BBX + posShape1BX) <= 0.1)
				shape1BXPlus = true;

		}
		if ((animShape1BBZ + posShape1BZ) < 0.95 && shape1BZPlus) {
			animShape1BBZ += 0.005;
		}
		else {
			shape1BZPlus = false;
			animShape1BBZ -= 0.005;
			if ((animShape1BBZ + posShape1BZ) <= 0.1)
				shape1BZPlus = true;

		}

	}
	if (animShape2B) {
		animShape2 += 0.005;
	}
	if (animShape3B) {
		if ((0.5 + animShape3) < 0.9 && shape3Plus) {
			animShape3 += 0.005;
		}
		else {
			shape3Plus = false;
			animShape3 -= 0.005;
			if ((animShape3 + 0.5) <= 0.1)
				shape3Plus = true;

		}
	}
	if (animSnowManB) {
		animSnowMan += 1;
	}
}
	else{
		animPresent += 0.005;
	}
	
	
		glutPostRedisplay();
		glutTimerFunc(1, Time, 0);
	
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Game");
	PlaySound(TEXT("Background.wav"), NULL, SND_ASYNC | SND_FILENAME);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutTimerFunc(0, Time, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
