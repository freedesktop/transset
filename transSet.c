/* Simple program to toggle Translucency property on next window clicked
   Uses dsimple.h and dsimple.c from xlsfonts.
   Based on xprops and xwininfo

   By Matthew Hawn
   Use however you want. I can't stop you.
*/



#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "dsimple.h"
#include <stdlib.h>


Window target_win;

/* needed by dsimple.c */

void usage()
{
  fprintf(stderr, "Bad arguments\n");
}

#define OPAQUE	0xffffffff
#define OPACITY	"_NET_WM_WINDOW_OPACITY"

/* nothing fancy */
int main(int argc, char **argv)
{
  int gotd = 0;
  double d;
  unsigned int opacity;

  /* wonderful utility */
  Setup_Display_And_Screen(&argc, argv);

  if (argv[1])
  {
    printf ("got arg %s\n", argv[1]);
    d = atof (argv[1]);
    printf ("d is %g\n", d);
    gotd = 1;
  }

  /* grab mouse and return window that is next clicked */
  target_win = Select_Window(dpy);

  unsigned char *data;

  Atom actual;
  int format;
  unsigned long n, left;

  if (gotd)
    opacity = (unsigned int) (d * OPAQUE);
  else
  {
    /* get property */
    XGetWindowProperty(dpy, target_win, XInternAtom(dpy, OPACITY, False), 
		       0L, 1L, False, XA_CARDINAL, &actual, &format, &n, &left, 
		 (unsigned char **) &data);
  
    if (data != None)
      {
	memcpy (&opacity, data, sizeof (unsigned int));
	XFree(( void *) data );
	printf("Found property: %g\n", (double) opacity / OPAQUE);
      }
    else
      opacity = OPAQUE;
  
    /* toggle */
    if (opacity != OPAQUE)
      opacity = OPAQUE;
    else
      opacity = 0xc0000000;
  }

  printf ("opacity 0x%x\n", opacity);
  if (opacity == OPAQUE)
    XDeleteProperty (dpy, target_win, XInternAtom(dpy, OPACITY, False));
  /* set it */
  else
    XChangeProperty(dpy, target_win, XInternAtom(dpy, OPACITY, False), 
		    XA_CARDINAL, 32, PropModeReplace, 
		    (unsigned char *) &opacity, 1L);
  XSync(dpy, False);
  printf("Set Property to %g\n", (double) opacity / OPAQUE);
  

  /* all done, wasn't that simple */
  return 0;
}
