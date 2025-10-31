#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../miniglut.h"

static const char *helpprompt[] = {"Press F1 for help", 0};
static const char *helptext[] = {
	"Rotate: left mouse drag",
	" Scale: right mouse drag up/down",
	"   Pan: middle mouse drag",
	"",
	"Toggle fullscreen: f",
	"Toggle animation: space",
	"Quit: escape",
	0
};

void idle(void);
void display(void);
void print_help(void);
void font_test(void);
void reshape(int x, int y);
void keypress(unsigned char key, int x, int y);
void skeypress(int key, int x, int y);
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

int win_width, win_height;
float cam_theta, cam_phi = 25, cam_dist = 8;
int mouse_x, mouse_y;
int bnstate[8];
int anim, help, showfonts;
float torus_pos[3], torus_rot[4] = {0, 0, 0, 1};
int teapot, torus, cone, sphere;
long nframes;

#ifndef GL_FRAMEBUFFER_SRGB
#define GL_FRAMEBUFFER_SRGB	0x8db9
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809d
#endif

int main(int argc, char **argv)
{
	int i, test_aa = 0, test_srgb = 0;
	unsigned int mode = GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE;

	for(i=1; i<argc; i++) {
		if(strcmp(argv[i], "-ms") == 0) {
			test_aa = 1;
		} else if(strcmp(argv[i], "-srgb") == 0) {
			test_srgb = 1;
		}
	}

	if(test_aa) mode |= GLUT_MULTISAMPLE;
	if(test_srgb) mode |= GLUT_SRGB;

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(mode);
	glutCreateWindow("miniglut test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(skeypress);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutSpaceballMotionFunc(sball_motion);
	glutSpaceballRotateFunc(sball_rotate);
	glutSpaceballButtonFunc(sball_button);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	if(test_aa) {
		glEnable(GL_MULTISAMPLE);
	}
	if(test_srgb) {
		glEnable(GL_FRAMEBUFFER_SRGB);
	}

	torus = glGenLists(1);
	glNewList(torus, GL_COMPILE);
	glutSolidTorus(0.3, 1, 16, 24);
	glEndList();

	cone = glGenLists(1);
	glNewList(cone, GL_COMPILE);
	glutSolidCone(1.1, 2, 16, 2);
	glEndList();

	sphere = glGenLists(1);
	glNewList(sphere, GL_COMPILE);
	glutSolidSphere(0.4, 16, 8);
	glEndList();

	teapot = glGenLists(1);
	glNewList(teapot, GL_COMPILE);
	glutSolidTeapot(1.0);
	glEndList();

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
	glCallList(torus);
	glPopMatrix();

	glCallList(sphere);

	glPushMatrix();
	glTranslatef(torus_pos[0] - 2.5, torus_pos[1], torus_pos[2]);
	mrotation_quat(sbrot_xform, torus_rot);
	glMultMatrixf(sbrot_xform);
	glutSolidCube(1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, -1, 0);
	glRotatef(-90, 1, 0, 0);
	glCallList(cone);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5, 2.5);
	glFrontFace(GL_CW);
	glCallList(teapot);
	glFrontFace(GL_CCW);
	glPopMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-5, -1.3, 5);
	glVertex3f(5, -1.3, 5);
	glVertex3f(5, -1.3, -5);
	glVertex3f(-5, -1.3, -5);
	glEnd();

	font_test();
	print_help();

	glutSwapBuffers();
	nframes++;
}

void print_help(void)
{
	int i;
	const char *s, **text;

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win_width, 0, win_height, -1, 1);

	text = help ? helptext : helpprompt;

	for(i=0; text[i]; i++) {
		glColor3f(0, 0.1, 0);
		glRasterPos2f(7, win_height - (i + 1) * 20 - 2);
		s = text[i];
		while(*s) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s++);
		}
		glColor3f(0, 0.9, 0);
		glRasterPos2f(5, win_height - (i + 1) * 20);
		s = text[i];
		while(*s) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s++);
		}
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void font_test(void)
{
	int x, y, ymax, i, j, font, len;
	static const char *text[] = {
		"A ","GLUT_BITMAP_9_BY_15"," brown fox jumped over the lazy dog",
		"A quick ","GLUT_BITMAP_8_BY_13"," fox jumped over the lazy dog",
		"A quick brown ","GLUT_BITMAP_TIMES_ROMAN_10"," jumped over the lazy dog",
		"A quick brown fox ","GLUT_BITMAP_TIMES_ROMAN_24"," over the lazy dog",
		"A quick brown fox jumped ","GLUT_BITMAP_HELVETICA_10"," the lazy dog",
		"A quick brown fox jumped over ","GLUT_BITMAP_HELVETICA_12"," lazy dog",
		"A quick brown fox jumped over the ","GLUT_BITMAP_HELVETICA_18"," dog"
	};
	const char **str = text;

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win_width, 0, win_height, -1, 1);

	if(!showfonts) {
		const char *s = "Press F2 for font test";
		x = win_width - glutBitmapLength(GLUT_BITMAP_9_BY_15, s) - 10;
		for(i=0; i<2; i++) {
			glColor3f(0, i ? 0.9 : 0.1, 0);
			glRasterPos2i(x - i * 2, win_height - 20 + i * 2);
			glutBitmapString(GLUT_BITMAP_9_BY_15, s);
		}

	} else {

		y = win_height / 2;
		ymax = y;
		for(i=0; i<7; i++) {
			font = GLUT_BITMAP_9_BY_15 + i;
			ymax -= glutBitmapHeight(font) + 10;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0, 0, 0, 0.7);
		glRectf(0, ymax - 10, win_width, y);
		glDisable(GL_BLEND);

		for(i=0; i<7; i++) {
			font = GLUT_BITMAP_9_BY_15 + i;
			y -= glutBitmapHeight(font) + 10;

			len = 0;
			for(j=0; j<3; j++) {
				len += glutBitmapLength(font, str[j]);
			}
			x = (win_width - len) / 2;

			for(j=0; j<3; j++) {
				if(j & 1) {
					glColor3f(1.0, 0.3, 0.28);
				} else {
					glColor3f(0.3, 0.6, 1.0);
				}
				glRasterPos2f(x, y);
				x += glutBitmapLength(font, *str);
				glutBitmapString(font, *str++);
			}
		}
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

#define ZNEAR	0.5f
void reshape(int x, int y)
{
	float vsz, aspect = (float)x / (float)y;
	win_width = x;
	win_height = y;

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
	static long start_msec;

	switch(key) {
	case 27:
	case 'q':
		glutExit();
		break;

	case ' ':
		anim ^= 1;
		glutIdleFunc(anim ? idle : 0);
		glutPostRedisplay();

		if(anim) {
			start_msec = glutGet(GLUT_ELAPSED_TIME);
			nframes = 0;
		} else {
			long tm = glutGet(GLUT_ELAPSED_TIME) - start_msec;
			long fps = (nframes * 100000) / tm;
			printf("framerate: %ld.%ld fps\n", fps / 100, fps % 100);
		}
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

void skeypress(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_F1:
		help ^= 1;
		glutPostRedisplay();
		break;

	case GLUT_KEY_F2:
		showfonts ^= 1;
		glutPostRedisplay();
		break;

	default:
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
