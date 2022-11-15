OUTFILE = brainfuck

INSTALLDIR = /usr/local/bin

MV = mv
STRIP = strip

all: src/main.c
	$(CC) -o $(OUTFILE) src/main.c $(CFLAGS) $(CPPFLAGS)
	$(STRIP) $(OUTFILE)

install: all
	$(MV) $(OUTFILE) $(INSTALLDIR)

uninstall:
	$(RM) $(INSTALLDIR)/$(OUTFILE)

.PHONY: all install uninstall

