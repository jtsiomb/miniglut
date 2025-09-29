MiniGLUT
========

![logo](http://nuclear.mutantstargoat.com/sw/miniglut/img/miniglut_logo_sm_alpha.png)

![GNU/Linux build status](https://github.com/jtsiomb/miniglut/actions/workflows/build_gnulinux.yml/badge.svg)
![Windows MinGW build status](https://github.com/jtsiomb/miniglut/actions/workflows/build_win_mingw.yml/badge.svg)
![Windows MSVC6 build status](https://github.com/jtsiomb/miniglut/actions/workflows/build_win_msvc6.yml/badge.svg)

MiniGLUT is a subset of GLUT (or more precisely a subset of FreeGLUT)
implemented in a single C source file, without any non-essential dependencies,
not even the C library.

You can use MiniGLUT by simply dropping two files: `miniglut.h` and `miniglut.c`
into your project source tree, or by building MiniGLUT as a static library and
linking with it.

MiniGLUT does not intend to replace a full GLUT library, like FreeGLUT, for
hacking small to medium OpenGL programs. The purpose of MiniGLUT is to
potentially replace a full GLUT library when it's time for release, in order to
minimize runtime dependencies of the resulting binary.

A second reason to use MiniGLUT is to ease porting of UNIX OpenGL programs to
Windows, especially when using the microsoft compiler, where setting up and
linking with a proper 3rd-party library is an ordeal in itself.  Even more so if
you decide to statically link, at which point you need to deal with the whole
"MSVC runtime" chaos.

On GNU/Linux x86/x86-64 and 32bit Windows, MiniGLUT can be compiled to never
call any C library functions whatsoever (which is the default if you use the
included makefile/msvc project to build a static library). This is useful to
avoid dependencies on any specific libc or msvc runtime.

Download
--------
Latest release (v0.5):
  - https://github.com/jtsiomb/miniglut/releases/download/v0.5/miniglut-0.5.tar.gz
  - https://github.com/jtsiomb/miniglut/releases/download/v0.5/miniglut-0.5.zip

Both archives are identical, choose whichever is more convenient.

Source repository (git):
  - https://github.com/jtsiomb/miniglut

Build
-----
Under X11 MiniGLUT depends only on Xlib and OpenGL. Therefore to build a program
using MiniGLUT you'll need to link with `-lX11 -lGL`.

Under Windows MiniGLUT depends only on OpenGL, GDI and winmm. Therefore to build
a program using MiniGLUT you'll need to link with `-lopengl32 -lgdi32 -lwinmm`.
When building with MSVC, linking with the correct libraries is taken care by
pragmas in the header file. If you wish to avoid the winmm dependency, define
`MINIGLUT_NO_WINMM`.

To disable calling any C library functions, make sure to have `MINIGLUT_NO_LIBC`
defined when building `miniglut.c`. Either add that to your build system, or
just modify `miniglut.c` and define it at the top.

> Note: in previous versions (including v0.5), building without libc was the
> default, and you had to define `MINIGLUT_USE_LIBC` to make it use libc. But
> it turns out usually when you're building miniglut as part of your project,
> there's no real downside to using libc in most use cases, so I decided to
> change the default, and have the extra define go to the static library build
> files instead of *every* project which drops `miniglut.h`/`miniglut.c` in the
> source tree.

To avoid calling C library functions, MiniGLUT uses inline assembly code for
system calls and trigonometric operations. This is currently implemented only
on x86 (32 and 64bit), and only on 32bit when building with MSVC (which doesn't
support inline assembly on x86-64). For all other systems you need to link with
libc.

License
-------
Copyright (C) 2020-2024 John Tsiombikas <nuclear@member.fsf.org>

MiniGLUT is free software. Feel free to use, modify and/or redistribute it,
under the terms of the GNU General Public License v3, or at your option any
newer version published by the Free Software Foundation. See COPYING for
details.

The intention is not to dictate a specific free software license (GPL) but to
shut the door to proprietary programs. If you want to use MiniGLUT in a free
software project with an incompatible license, contact me and we will figure out
a way to enable that.

To learn more about GPL-incompatible free software licenses where this might
be an issue, see:
https://www.gnu.org/licenses/license-list.en.html#GPLIncompatibleLicenses

Implementation Notes
--------------------
On UNIX systems, spaceball callbacks are supported by talking to a 6dof device
driver through the *Magellan X11 ClientMessage protocol*. This works with
either the free software *spacenavd* driver, or the proprietary *3dxsrv*.
Spacenavd supports all 6dof devices from the first serial ones to current
models.

On Windows, spaceball support relies on the 3Dconnexion driver and its old
siapp API. This should work on any version of the driver from very old ones
running on windows 9x and supporting serial devices, to the latest current
driver for USB and bluetooth spacemice.

Known Issues
------------
MiniGLUT being a subset of GLUT, is missing a number of features. Some of them
on purpose to keep it minimal, and some of them because I didn't happen to use
them in a program I wanted to link with MiniGLUT yet.

Missing GLUT features:
 - The only supported systems are: UNIX with X11 (GLX), and Windows (WGL).
 - Multiple windows.
 - Subwindows.
 - Overlays.
 - Game mode (video mode switching).
 - Menus.
 - Stroke fonts.
 - Bitmap fonts are currently not available on Windows.
 - Some of the primitives.
 - Buttons and Dials callbacks.
 - Tablet callbacks.
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
better if you are volunteering to implement it yourself, send me an email at:
nuclear@member.fsf.org

Only plain-text emails, hard-wrapped at 72 columns will be accepted.
