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


Window target_win;

/* needed by dsimple.c */

void usage()
{
  fprintf(stderr, "Bad arguments\n");
}


/* nothing fancy */
int main(int argc, char **argv)
{

  /* wonderful utility */
  Setup_Display_And_Screen(&argc, argv);

  /* grab mouse and return window that is next clicked */
  target_win = Select_Window(dpy);

  int val = 15;

  unsigned char *data;

  Atom actual;
  int format;
  unsigned long n, left;

  /* get property */
  XGetWindowProperty(dpy, target_win, XInternAtom(dpy, "CM_TRANSLUCENCY", False), 
		     0L, 1L, False, XA_INTEGER, &actual, &format, &n, &left, 
	       (unsigned char **) &data);

  if (data != None)
    {
      
      printf("Found property: %d\n", (int) *data );

      /* toggle */
      if ((int) *data > 0)
	val = 0;

      XFree(( void *) data );

    }


  /* set it */
  XChangeProperty(dpy, target_win, XInternAtom(dpy, "CM_TRANSLUCENCY", False), 
		  XA_INTEGER, 32, PropModeReplace, 
		  (unsigned char *) &val, 1L);
  XSync(dpy, False);
  printf("Set Property to %d\n", val);
  

  /* all done, wasn't that simple */
  return 0;
}
