#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../miniglut.h"

#if defined(unix) || defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

void display(void);
void reshape(int x, int y);
void keypress(unsigned char key, int x, int y);

int interv = 1;
int win_width, win_height;
long delay;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("vsync test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(glutPostRedisplay);
	glutKeyboardFunc(keypress);

	glutSwapInterval(interv);

	glutMainLoop();
	return 0;
}

void display(void)
{
	static long nframes, prev_fps_upd;
	static int prev_interv = INT_MIN;
	static char fpsbuf[128], text[128];
	long upd_time, msec = glutGet(GLUT_ELAPSED_TIME);
	float t = (float)msec / 1000.0f;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(sin(t * 8.0f), 0, 0);
	glScalef(0.5, 0.5, 0.5);

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.3, 0.2);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);
	glEnd();

	if((upd_time = msec - prev_fps_upd) >= 1000) {
		float fps = nframes * 1000.0f / (float)upd_time;
		nframes = 0;
		prev_fps_upd = msec;
		sprintf(fpsbuf, "%.1f fps", fps);
	}

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win_width, 0, win_height, -1, 1);

	glColor3f(0.2, 1, 0.2);
	glRasterPos2i(5, win_height - 20);
	sprintf(text, "swap interval: %d", interv);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, text);

	glRasterPos2i(5, win_height - 40);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, fpsbuf);

	if(delay) {
		glRasterPos2i(5, win_height - 60);
		sprintf(text, "delay: %ld ms", delay);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, text);
	}

	glPopMatrix();

	if(delay) {
#if defined(unix) || defined(__unix__)
		usleep(delay * 1000);
#elif defined(_WIN32)
		Sleep(delay);
#endif
	}

	glutSwapBuffers();
	nframes++;
}

void reshape(int x, int y)
{
	win_width = x;
	win_height= y;
	glViewport(0, 0, x, y);
}

void keypress(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);

	case '=':
		delay += 5;
		break;
	case '-':
		delay -= 5;
		if(delay < 0) delay = 0;
		break;

	default:
		if(key >= '0' && key <= '9') {
			interv = key - '0';
			if(glutGetModifiers() & GLUT_ACTIVE_CTRL) {
				interv = -interv;
			}
			glutSwapInterval(interv);
		}
	}
}
