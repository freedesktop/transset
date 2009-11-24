bin_PROGRAMS = transset
man_MANS = transset.1

AM_CFLAGS = $(CWARNFLAGS) $(TRANSSET_CFLAGS)
transset_LDADD = $(TRANSSET_LIBS) -lm

transset_SOURCES = transSet.c dsimple.c dsimple.h

EXTRA_DIST = \
	autogen.sh \
	ChangeLog \
	${man_MANS}

MAINTAINERCLEANFILES=ChangeLog

.PHONY: ChangeLog

ChangeLog:
	$(CHANGELOG_CMD)

dist-hook: ChangeLog

