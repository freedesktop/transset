PACKAGES=xcomposite xfixes xdamage xrender
LIBS=`pkg-config --libs ${PACKAGES}` -lm
INCS=`pkg-config --cflags ${PACKAGES}`

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $*.c

OBJS=transSet.o dsimple.o

transset: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS): dsimple.h

clean:
	rm -f $(OBJS) transset
