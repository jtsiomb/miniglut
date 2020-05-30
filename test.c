#include <math.h>
#include "miniglut.h"

void idle(void);
void display(void);
void reshape(int x, int y);
void keypress(unsigned char key, int x, int y);
void mouse(int bn, int st, int x, int y);
void motion(int x, int y);
void sball_motion(int x, int y, int z);
void sball_rotate(int rx, int ry, int rz);
void sball_button(int bn, int state);

static void vcross(float *res, const float *a, const float *b);
static void qmul(float *a, const float *b);
static void qrotation(float *q, float angle, float x, float y, float z);
static void qrotate(float *q, float angle, float x, float y, float z);
static void mrotation_quat(float *m, const float *q);


float cam_theta, cam_phi = 25, cam_dist = 8;
int mouse_x, mouse_y;
int bnstate[8];
int anim;
float torus_pos[3], torus_rot[4] = {0, 0, 0, 1};


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("miniglut test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpaceballMotionFunc(sball_motion);
	glutSpaceballRotateFunc(sball_rotate);
	glutSpaceballButtonFunc(sball_button);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutMainLoop();
	return 0;
}

void idle(void)
{
	glutPostRedisplay();
}

void display(void)
{
	long tm;
	float lpos[] = {-1, 2, 3, 0};
	float sbrot_xform[16];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -cam_dist);
	glRotatef(cam_phi, 1, 0, 0);
	glRotatef(cam_theta, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	glPushMatrix();
	if(anim) {
		tm = glutGet(GLUT_ELAPSED_TIME);
		glRotatef(tm / 10.0f, 1, 0, 0);
		glRotatef(tm / 10.0f, 0, 1, 0);
	}
	glutSolidTorus(0.3, 1, 16, 24);
	glPopMatrix();

	glutSolidSphere(0.4, 16, 8);

	glPushMatrix();
	glTranslatef(torus_pos[0] - 2.5, torus_pos[1], torus_pos[2]);
	mrotation_quat(sbrot_xform, torus_rot);
	glMultMatrixf(sbrot_xform);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, -1, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(1.1, 2, 16, 2);
	glPopMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5, -1.3, 5);
	glVertex3f(5, -1.3, 5);
	glVertex3f(5, -1.3, -5);
	glVertex3f(-5, -1.3, -5);
	glEnd();

	glutSwapBuffers();
}

#define ZNEAR	0.5f
void reshape(int x, int y)
{
	float vsz, aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	vsz = 0.4663f * ZNEAR;
	glFrustum(-aspect * vsz, aspect * vsz, -vsz, vsz, 0.5, 500.0);
}

void keypress(unsigned char key, int x, int y)
{
	static int fullscr;
	static int prev_xsz, prev_ysz;

	switch(key) {
	case 27:
	case 'q':
		glutExit();
		break;

	case ' ':
		anim ^= 1;
		glutIdleFunc(anim ? idle : 0);
		glutPostRedisplay();
		break;

	case '\n':
	case '\r':
		if(!(glutGetModifiers() & GLUT_ACTIVE_ALT)) {
			break;
		}
	case 'f':
		fullscr ^= 1;
		if(fullscr) {
			prev_xsz = glutGet(GLUT_WINDOW_WIDTH);
			prev_ysz = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		} else {
			glutReshapeWindow(prev_xsz, prev_ysz);
		}
		break;
	}
}

void mouse(int bn, int st, int x, int y)
{
	int bidx = bn - GLUT_LEFT_BUTTON;
	bnstate[bidx] = st == GLUT_DOWN;
	mouse_x = x;
	mouse_y = y;
}

void motion(int x, int y)
{
	int dx = x - mouse_x;
	int dy = y - mouse_y;
	mouse_x = x;
	mouse_y = y;

	if(!(dx | dy)) return;

	if(bnstate[0]) {
		cam_theta += dx * 0.5;
		cam_phi += dy * 0.5;
		if(cam_phi < -90) cam_phi = -90;
		if(cam_phi > 90) cam_phi = 90;
		glutPostRedisplay();
	}
	if(bnstate[2]) {
		cam_dist += dy * 0.1;
		if(cam_dist < 0) cam_dist = 0;
		glutPostRedisplay();
	}
}

void sball_motion(int x, int y, int z)
{
	torus_pos[0] += x * 0.001f;
	torus_pos[1] += y * 0.001f;
	torus_pos[2] -= z * 0.001f;
	glutPostRedisplay();
}

static float rsqrt(float number)
{
	int i;
	float x2, y;
	static const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(int*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y *= threehalfs - (x2 * y * y);
	y *= threehalfs - (x2 * y * y);
	return y;
}

void sball_rotate(int rx, int ry, int rz)
{
	if(rx | ry | rz) {
		float s = (float)rsqrt(rx * rx + ry * ry + rz * rz);
		qrotate(torus_rot, 0.001f / s, rx * s, ry * s, -rz * s);
		glutPostRedisplay();
	}
}

void sball_button(int bn, int state)
{
	if(state == GLUT_DOWN) {
		torus_pos[0] = torus_pos[1] = torus_pos[2] = 0;
		torus_rot[0] = torus_rot[1] = torus_rot[2] = 0;
		torus_rot[3] = 1;
		glutPostRedisplay();
	}
}


static void vcross(float *res, const float *a, const float *b)
{
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

static void qmul(float *a, const float *b)
{
	float x, y, z, dot;
	float cross[3];

	dot = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	vcross(cross, a, b);

	x = a[3] * b[0] + b[3] * a[0] + cross[0];
	y = a[3] * b[1] + b[3] * a[1] + cross[1];
	z = a[3] * b[2] + b[3] * a[2] + cross[2];
	a[3] = a[3] * b[3] - dot;
	a[0] = x;
	a[1] = y;
	a[2] = z;
}

void mglut_sincos(float angle, float *sptr, float *cptr);
float mglut_tan(float x);

static void qrotation(float *q, float angle, float x, float y, float z)
{
	float sa, ca;
	mglut_sincos(angle * 0.5f, &sa, &ca);
	q[3] = ca;
	q[0] = x * sa;
	q[1] = y * sa;
	q[2] = z * sa;
}

static void qrotate(float *q, float angle, float x, float y, float z)
{
	float qrot[4];
	qrotation(qrot, angle, x, y, z);
	qmul(qrot, q);
	q[0] = qrot[0];
	q[1] = qrot[1];
	q[2] = qrot[2];
	q[3] = qrot[3];
}

static void mrotation_quat(float *m, const float *q)
{
	float xsq2 = 2.0f * q[0] * q[0];
	float ysq2 = 2.0f * q[1] * q[1];
	float zsq2 = 2.0f * q[2] * q[2];
	float sx = 1.0f - ysq2 - zsq2;
	float sy = 1.0f - xsq2 - zsq2;
	float sz = 1.0f - xsq2 - ysq2;

	m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	m[0] = sx;
	m[1] = 2.0f * q[0] * q[1] + 2.0f * q[3] * q[2];
	m[2] = 2.0f * q[2] * q[0] - 2.0f * q[3] * q[1];
	m[4] = 2.0f * q[0] * q[1] - 2.0f * q[3] * q[2];
	m[5] = sy;
	m[6] = 2.0f * q[1] * q[2] + 2.0f * q[3] * q[0];
	m[8] = 2.0f * q[2] * q[0] + 2.0f * q[3] * q[1];
	m[9] = 2.0f * q[1] * q[2] - 2.0f * q[3] * q[0];
	m[10] = sz;
}
