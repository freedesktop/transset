bin_PROGRAMS = transset
man_MANS = transset.1

AM_CFLAGS = $(CWARNFLAGS) $(TRANSSET_CFLAGS)
transset_LDADD = $(TRANSSET_LIBS) $(LIBOBJS)

transset_SOURCES = transSet.c dsimple.c dsimple.h

MAINTAINERCLEANFILES = ChangeLog INSTALL

.PHONY: ChangeLog INSTALL

INSTALL:
	$(INSTALL_CMD)

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog INSTALL
