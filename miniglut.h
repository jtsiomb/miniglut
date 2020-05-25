/*
MiniGLUT - minimal GLUT subset without dependencies
Copyright (C) 2020  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MINIGLUT_H_
#define MINIGLUT_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <GL/gl.h>

/* mode flags for glutInitDisplayMode */
#define GLUT_RGB			0
#define GLUT_RGBA			0
#define GLUT_INDEX			0x001
#define GLUT_SINGLE			0
#define GLUT_DOUBLE			0x002
#define GLUT_ACCUM			0x004
#define GLUT_ALPHA			0x008
#define GLUT_DEPTH			0x010
#define GLUT_STENCIL		0x020
#define GLUT_STEREO			0x040
#define GLUT_MULTISAMPLE	0x100
#define GLUT_SRGB			0x200

enum { GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON };
enum { GLUT_UP, GLUT_DOWN };
enum { GLUT_NOT_VISIBLE, GLUT_VISIBLE };
enum { GLUT_LEFT, GLUT_ENTERED };

/* cursors */
enum {
	GLUT_CURSOR_INHERIT,
	GLUT_CURSOR_LEFT_ARROW,
	GLUT_CURSOR_NONE
};

/* glutGet */
enum {
	GLUT_WINDOW_X,
	GLUT_WINDOW_Y,
	GLUT_WINDOW_WIDTH,
	GLUT_WINDOW_HEIGHT,
	GLUT_WINDOW_BUFFER_SIZE,
	GLUT_WINDOW_STENCIL_SIZE,
	GLUT_WINDOW_DEPTH_SIZE,
	GLUT_WINDOW_RED_SIZE,
	GLUT_WINDOW_GREEN_SIZE,
	GLUT_WINDOW_BLUE_SIZE,
	GLUT_WINDOW_ALPHA_SIZE,
	GLUT_WINDOW_DOUBLEBUFFER,
	GLUT_WINDOW_RGBA,
	GLUT_WINDOW_NUM_SAMPLES,
	GLUT_WINDOW_STEREO,
	GLUT_WINDOW_SRGB,
	GLUT_WINDOW_CURSOR,
	GLUT_SCREEN_WIDTH,
	GLUT_SCREEN_HEIGHT,
	GLUT_INIT_DISPLAY_MODE,
	GLUT_INIT_WINDOW_X,
	GLUT_INIT_WINDOW_Y,
	GLUT_INIT_WINDOW_WIDTH,
	GLUT_INIT_WINDOW_HEIGHT,
	GLUT_ELAPSED_TIME
};

/* returned by glutGetModifiers */
#define GLUT_ACTIVE_SHIFT	1
#define GLUT_ACTIVE_CTRL	2
#define GLUT_ACTIVE_ALT		4

typedef void (*glut_cb)(void);
typedef void (*glut_cb_reshape)(int x, int y);
typedef void (*glut_cb_state)(int state);
typedef void (*glut_cb_keyb)(unsigned char key, int x, int y);
typedef void (*glut_cb_special)(int key, int x, int y);
typedef void (*glut_cb_mouse)(int bn, int state, int x, int y);
typedef void (*glut_cb_motion)(int x, int y);
typedef void (*glut_cb_sbmotion)(int x, int y, int z);
typedef void (*glut_cb_sbbutton)(int bn, int state);


void glutInit(int *argc, char **argv);
void glutInitWindowPosition(int x, int y);
void glutInitWindowSize(int xsz, int ysz);
void glutInitDisplayMode(unsigned int mode);
void glutCreateWindow(const char *title);

void glutExit(void);
void glutMainLoop(void);
void glutMainLoopEvent(void);

void glutPostRedisplay(void);
void glutSwapBuffers(void);
void glutPositionWindow(int x, int y);
void glutReshapeWindow(int xsz, int ysz);
void glutFullScreen(void);
void glutSetWindowTitle(const char *title);
void glutSetIconTitle(const char *title);
void glutSetCursor(int cursor);

void glutIdleFunc(glut_cb func);
void glutDisplayFunc(glut_cb func);
void glutReshapeFunc(glut_cb_reshape func);
void glutVisibilityFunc(glut_cb_state func);
void glutEntryFunc(glut_cb_state func);
void glutKeyboardFunc(glut_cb_keyb func);
void glutKeyboardUpFunc(glut_cb_keyb func);
void glutSpecialFunc(glut_cb_special func);
void glutSpecialUpFunc(glut_cb_special func);
void glutMouseFunc(glut_cb_mouse func);
void glutMotionFunc(glut_cb_motion func);
void glutPassiveMotionFunc(glut_cb_motion func);
void glutSpaceballMotionFunc(glut_cb_sbmotion func);
void glutSpaceballRotateFunc(glut_cb_sbmotion func);
void glutSpaceballBittonFunc(glut_cb_sbbutton func);

int glutGet(unsigned int s);
int glutGetModifiers(void);
int glutExtensionSupported(char *ext);

void glutSolidSphere(float rad);
void glutWireSphere(float rad);
void glutSolidCube(float sz);
void glutWireCube(float sz);
void glutSolidTorus(float inner_rad, float outer_rad, float sides, float rings);
void glutWireTorus(float inner_rad, float outer_rad, float sides, float rings);
void glutSolidTeapot(float size);
void glutWireTeapot(float size);

#endif	/* MINIGLUT_H_ */
