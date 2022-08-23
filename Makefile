PREFIX = /usr/local

olib = miniglut.o
otest = test.o
alib = libminiglut.a
bin = test

CFLAGS = -pedantic -Wall -g

isx86 ?= $(shell uname -m | sed 's/x86_64/x86/; s/i.86/x86/')

sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/; s/IRIX.*/IRIX/')
ifeq ($(sys), mingw)
	olib = miniglut.w32.o
	otest = test.w32.o
	alib = libminiglut-w32.a
	bin = test.exe

	LDFLAGS = -mconsole -lopengl32 -lgdi32 -lwinmm
else
	ifeq ($(sys)-$(isx86), Linux-x86)
		CFLAGS += -I/usr/X11R6/include
		LDFLAGS = -L/usr/X11R6/lib -lX11 -lGL
	else
		# for other UNIX or non-x86 where sys_ and trig functions are not
		# implemented, just use libc
		CFLAGS += -DMINIGLUT_USE_LIBC
		LDFLAGS = -lX11 -lGL -lm
		ifeq ($(sys), IRIX)
			CC = gcc
		endif
	endif
endif

$(bin): $(otest) $(alib)
	$(CC) -o $@ $(otest) $(alib) $(LDFLAGS)

$(alib): $(olib)
	$(AR) rcs $@ $(olib)

%.w32.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(alib) $(olib) $(otest) $(bin)

.PHONY: install
install: $(alib)
	mkdir -p $(DESTDIR)$(PREFIX)/include $(DESTDIR)$(PREFIX)/lib
	cp miniglut.h $(DESTDIR)$(PREFIX)/include/miniglut.h
	cp $(alib) $(DESTDIR)$(PREFIX)/lib/libminiglut.a

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/include/miniglut.h
	rm -f $(DESTDIR)$(PREFIX)/lib/libminiglut.a

.PHONY: cross
cross:
	$(MAKE) CC=i686-w64-mingw32-gcc sys=mingw

.PHONY: cross-clean
cross-clean:
	$(MAKE) CC=i686-w64-mingw32-gcc sys=mingw clean
