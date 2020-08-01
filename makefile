INSTALLDIR = /usr/local/bin

make: cursive.c config.h
	gcc -o cursive cursive.c `pkg-config --cflags --libs ncurses`

install: cursive
	mkdir -p ${INSTALLDIR}
	cp -f cursive ${INSTALLDIR}/cursive
	chmod 755 ${INSTALLDIR}/cursive

uninstall:
	rm -f ${INSTALLDIR}/cursive
