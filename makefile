INSTALLDIR = /usr/local/bin
MANDIR = /usr/local/share/man
CC = tcc
CFLAGS = -lcurses

make: cursive.c config.h
	${CC} ${CFLAGS} -o cursive cursive.c

install: cursive cursive.1
	mkdir -p ${INSTALLDIR}
	cp -f cursive ${INSTALLDIR}/cursive
	chmod 755 ${INSTALLDIR}/cursive
	mkdir -p ${MANDIR}/man1
	cp -f cursive.1 ${MANDIR}/man1/cursive.1
	chmod 644 ${MANDIR}/man1/cursive.1

uninstall:
	rm -f ${INSTALLDIR}/cursive
	rm -f ${MANDIR}/man1/cursive.1
