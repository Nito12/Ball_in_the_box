/* ----------------------------------------------------------- */
/* Graph.-DV-Praktikum Ball in the box                         */
/* ----------------------------------------------------------- */
/* Last Update:    02.11.2013					C. D.          */
/* ----------------------------------------------------------- */

#include <iostream>      
#include <GL/freeglut.h>	// also loads glut.h and gl.h

using namespace std;
const float bounciness = 0.3;
const float sensibility = 0.00005;
const float friction = 0.9999;
float x = 0.0, z = 0.0, vx = 0.0, vz = 0.0;
struct surface{ float alpha_x = 0.0; float alpha_z = 0.0; bool x_up = false, x_down = false, z_up = false, z_down = false; };
surface s;
GLuint texture;

void init();
void render();
void resize(int width, int height);
void keyboard(unsigned char c, int x, int y);
void handle_arrow_down(int, int, int);
void handle_arrow_up(int, int, int);
void animate(int value);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Ball in the box");

	glutDisplayFunc(render);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(handle_arrow_down);
	glutSpecialUpFunc(handle_arrow_up);

	glutTimerFunc(10, animate, 0);	// register TimerCallback: execute after 10 msec with parameter 0
	
	init();
	glutMainLoop();

	return 0;
}


void init()
{
	glClearColor(0.8f, 0.9f, 1.0f, 1);	// light blue background
	glClearDepth(1.0);

	float mat_spec[] = { 1, 1, 1, 1 };
	float mat_shine[] = { 50 };
	float lpos[] = { 0, 10, 1, 0 };
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	glEnable(GL_DEPTH_TEST);	// remove hidden surfaces
	glEnable(GL_COLOR_MATERIAL);// make glColor affect material color -> prevent greyscale caused by lighting
	glEnable(GL_LIGHTING);		// enable lighting
	glEnable(GL_LIGHT0);		// enable

	//texture = SOIL_load_OGL_texture("wood.bmp",	SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear buffer
	glLoadIdentity();
	gluLookAt(	0.0, 5.0, 5.0,	// eye position
				0.0, 0.0, 0.0,	// center of focus
				0.0, 1.0, 0.0);	// up direction
	
	glBegin(GL_POLYGON);	// floor
		glColor4f(0.3f, 1.0f, 0.0f, 1.0f);
		glVertex3f(10, -2, 10);
		glColor4f(0.3f, 1.0f, 0.0f, 1.0f);
		glVertex3f(10, -2, -10);
		glColor4f(0.3f, 1.0f, 0.0f, 1.0f);
		glVertex3f(-10, -2, -10);
		glColor4f(0.3f, 1.0f, 0.0f, 1.0f);
		glVertex3f(-10, -2, 10);
	glEnd();

	glRotatef(s.alpha_x, 1, 0, 0); // surface inclination 
	glRotatef(s.alpha_z, 0, 0, 1);
	
	glBegin(GL_POLYGON); // box floor
		glColor4f(0.5, 0.5, 0.5, 1);
		glVertex3f(-1.5, -0.05, -1);
		glVertex3f(1.5, -0.05, -1);
		glVertex3f(1.5, -0.05, 1);
		glVertex3f(-1.5, -0.05, 1);
	glEnd();
	glPushMatrix(); // ball
		glTranslatef(x, 0, z);
		glScalef(0.05, 0.05, 0.05);
		glutSolidSphere(1, 30, 30);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 0, -1);
		glScalef(3, 0.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-1.5, 0, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(2.1, 0.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, 0, 1);
		glScalef(3, 0.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();	// right wall
		glTranslatef(1.5, 0, 0);
		glRotatef(90, 0, 1, 0);
		glScalef(2.1, 0.1, 0.1);
		glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
		glEnable(GL_TEXTURE_GEN_T);
		glBindTexture(GL_TEXTURE_2D, texture);
		glutSolidCube(1);
		glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
		glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();

	glutSwapBuffers();
}

void resize(int width, int height)	// handle window resize
{
	glMatrixMode(GL_PROJECTION); // Matrix für Transf.: Frustum->Viewport
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.5, 12.0);
	glMatrixMode(GL_MODELVIEW); // Modellierungs-/Viewing-Matrix
}

void keyboard(unsigned char c, int x, int y)
{
	if (c == 27) exit(0);			// Esc
}

void handle_arrow_down(int key, int x, int y)
{
	if		(key == GLUT_KEY_LEFT)	s.z_up = true;		// left
	else if (key == GLUT_KEY_UP)	s.x_down = true;	// up
	else if (key == GLUT_KEY_RIGHT)	s.z_down = true;	// right
	else if (key == GLUT_KEY_DOWN)	s.x_up = true;		// down
}
void handle_arrow_up(int key, int x, int y)
{
	if		(key == GLUT_KEY_LEFT)	s.z_up = false;		// left
	else if (key == GLUT_KEY_UP)	s.x_down = false;	// up
	else if (key == GLUT_KEY_RIGHT)	s.z_down = false;	// right
	else if (key == GLUT_KEY_DOWN)	s.x_up = false;		// down
}

void animate(int value)
{
	// increase angles:
	if		(s.x_up)	s.alpha_x++;
	else if (s.x_down)	s.alpha_x--;
	if		(s.z_up)	s.alpha_z++;
	else if (s.z_down)	s.alpha_z--;
	
	// limit angles to +/-30°:
	if		(s.alpha_x > 30)	s.alpha_x = 30;
	else if (s.alpha_x < -30)	s.alpha_x = -30;
	if		(s.alpha_z > 30)	s.alpha_z = 30;
	else if (s.alpha_z < -30)	s.alpha_z = -30;

	// calculate speed impact from angles:
	vx -= sensibility*s.alpha_z;
	vz += sensibility*s.alpha_x;
	vx *= friction;
	vz *= friction;

	// change position according to speed:
	x += vx;
	z += vz;

	// stop @ walls:
	if (x < -1.4 || x > 1.4)
	{
		if (x > 1.4) x = 1.4;
		else if (x < -1.4) x = -1.4;
		vx = -vx*bounciness; // bounce back hitting wall
	}
	if (z < -0.9 || z > 0.9)
	{
		if (z > 0.9) z = 0.9;
		else if (z < -0.9) z = -0.9;
		vz = -vz*bounciness;
	}
	
	glutPostRedisplay();
	glutTimerFunc(10, animate, ++value);
}