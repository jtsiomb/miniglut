PREFIX = /usr/local

olib = miniglut.o
otest = test.o
alib = libminiglut.a
bin = test

CFLAGS = -O3 -g3
LDFLAGS = -lGL -lX11 -lm

$(bin): $(otest) $(alib)
	$(CC) -o $@ $(otest) $(alib) $(LDFLAGS)

$(alib): $(olib)
	$(AR) rcs $@ $(olib)

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
