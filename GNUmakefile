PREFIX = /usr/local

olib = miniglut.o
alib = libminiglut.a
tests = 3dview vsync

CFLAGS = -O3 -g3

isx86 ?= $(shell uname -m | sed 's/x86_64/x86/; s/i.86/x86/')

sys ?= $(shell uname -s | sed 's/MINGW.*/mingw/; s/IRIX.*/IRIX/')
ifeq ($(sys), mingw)
	# on windows/mingw we can build without linking to libc
	CFLAGS += -DMINIGLUT_NO_LIBC
	LDFLAGS = -mconsole -lopengl32 -lgdi32 -lwinmm
	tests = 3dview.exe vsync.exe
else
	ifeq ($(sys)-$(isx86), Linux-x86)
		# for Linux x86/x86-64 we can build without linking to libc
		CFLAGS += -I/usr/X11R6/include -DMINIGLUT_NO_LIBC
		LDFLAGS = -L/usr/X11R6/lib -lX11 -lGL
	else
		# for other UNIX or non-x86 where sys_ and trig functions are not
		# implemented, just use libc
		LDFLAGS = -lX11 -lGL -lm
	endif
endif

.PHONY: all
all: $(alib) $(tests)

3dview: tests/3dview.o $(alib)
	$(CC) -o $@ $< $(alib) $(LDFLAGS)

vsync: tests/vsync.o $(alib)
	$(CC) -o $@ $< $(alib) $(LDFLAGS)

$(alib): $(olib)
	$(AR) rcs $@ $(olib)

.PHONY: clean
clean:
	rm -f $(alib) $(olib) $(tests) tests/*.o

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
