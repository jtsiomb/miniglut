MiniGLUT
========
MiniGLUT is a subset of GLUT (or more precisely a subset of FreeGLUT)
implemented in a single C source file, without any non-essential dependencies,
not even the C library.

You can use MiniGLUT by simply dropping two files: `miniglut.h` and `miniglut.c`
into your project source tree, or by building MiniGLUT as a static library and
linking with it.

MiniGLUT does not intend to replace a proper GLUT library like FreeGLUT for
hacking OpenGL experiments and small programs. The purpose of MiniGLUT is to
replace a proper GLUT library when it's time for release, in order to minimize
runtime dependencies of the resulting binary.

A second reason is for porting UNIX OpenGL programs to Windows, especially when
using the microsoft compiler, where setting up and linking with a proper
3rd-party library like FreeGLUT is an ordeal in itself. Even more so if you
decide to statically link, at which point you need to deal with the whole "MSVC
runtime" chaos. Since MiniGLUT does not call any C library functions, even if
you decide to link it as a static library, it will not affect your choice of
MSVC runtime library.

Missing features
----------------
MiniGLUT being a subset of GLUT, is missing a number of features. Some of them
on purpose to keep it minimal, and some of them because I didn't happen to use
them in a program I wanted to link with MiniGLUT yet.

Missing GLUT features:
 - Only supported systems are UNIX with X11 (GLX) and Windows (WGL).
 - Indexed color contexts.
 - Multiple windows.
 - Subwindows.
 - Overlays.
 - Game mode (video mode switching).
 - Menus.
 - Font rendering.
 - Some of the primitives.
 - Buttons and Dials callbacks.
 - Tablet callbacks.
 - Spaceball callbacks are only implemented on UNIX/X11.
 - Timer callback.

Missing FreeGLUT features:
 - Mobile callbacks.
 - Context version and profile selection and other context flags.
 - Window close actions.
 - Multi-touch/multi-pointer callbacks.
 - User-pointer callbacks.
 - Joystick callbacks.
 - More missing primitives.

If wish to let me know how much you need one of the missing features, or even
better if you are volunteering to implement it yourself, contact me through
through email at: nuclear@member.fsf.org

Only plain-text emails, hard-wrapped at 72 columns will be accepted.

License
-------
Copyright (C) 2020 John Tsiombikas <nuclear@member.fsf.org>

MiniGLUT is free software. Feel free to use, modify and/or redistribute it,
under the terms of the GNU General Public License v3, or at your option any
newer version published by the Free Software Foundation. See COPYING for
details.

The intention is not to dictate a specific free software license (GPL) but to
shut the door to proprietary programs. If you want to use MiniGLUT in a free
software project with an incompatible free software license, contact me, and we
will figure out a way to enable that. Usually only other copyleft licenses are
incompatible with the GPL. There's no issue with combining GPL code with code
using other non-copyleft free software licenses like MIT/X11, 3-clause BSD, and
so on.
See: https://www.gnu.org/licenses/license-list.en.html#GPLIncompatibleLicenses
