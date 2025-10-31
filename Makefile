PREFIX = /usr/local

olib = miniglut.o
alib = libminiglut.a
tests = 3dview vsync

CFLAGS = -O3 -g3
LDFLAGS = -lGL -lX11 -lm

.PHONY: all
all: $(alib) $(tests)

3dview: tests/3dview.o $(alib)
	$(CC) -o $@ tests/3dview.o $(alib) $(LDFLAGS)

vsync: tests/vsync.o $(alib)
	$(CC) -o $@ tests/vsync.o $(alib) $(LDFLAGS)

$(alib): $(olib)
	$(AR) rcs $@ $(olib)

.c.o:
	$(CC) -o $@ $(CFLAGS) -c $<

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
