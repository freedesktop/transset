/* Simple program to toggle Translucency property
   Based on 'transset' by  Matthew Hawn
   With some additional features to make it more automatic and integrated
   The purpos is to bind it from your wm to a key or mouse-button

   Use however you want.
   Written by: Daniel Forchheimer (upiom)
   
*/

#define VERSIONSTR "1"

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "dsimple.h"
#include <stdlib.h>
#include <getopt.h>


Window target_win;

/* needed by dsimple.c */

void usage()
{
  fprintf(stderr,"usage: transset-df [-options ...] [opacity]\n");
  fprintf(stderr,"options:\n");

  fprintf(stderr,
	  "    -h, --help           display this message\n");
  fprintf(stderr,
	  "    -t, --toggle         force toggle of opacity\n");
  fprintf(stderr,
	  "    -c, --click          select by clicking on window (default)\n");
  fprintf(stderr,
	  "    -p, --point          select the window currently under the cursor\n");
  fprintf(stderr,
	  "    -v, --version        print version number\n");

  exit(1);
}

#define OPAQUE	0xffffffff
#define OPACITY	"_NET_WM_WINDOW_OPACITY"

/* nothing fancy */
int main(int argc, char **argv)
{
  int gotd = 0;
  double d;
  unsigned int opacity;
  unsigned int current_opacity;
  int select_method = 0; // 0 = click, 1 = point
  int flag_toggle=0;
  int o;

  int options_index=0;
  static struct option long_options[] = {
    {"toggle",0,NULL,'t'},
    {"help",0,NULL,'h'},
    {"point",0,NULL,'p'},
    {"click",0,NULL,'c'},
    {"version",0,NULL,'v'},
    {0,0,0,0}
  };

  /* wonderful utility */
  Setup_Display_And_Screen(&argc, argv);

  while ((o = getopt_long(argc, argv, "thpcv",long_options,&options_index)) != -1)
    {
      switch (o) {
      case 't':
	flag_toggle=1;
	break;
      case 'h':
	usage();
	break;
      case 'c':
	select_method=0;
	break;
      case 'p':
	select_method=1;
	break;
      case 'v':
	fprintf(stderr,"version: %s\n",VERSIONSTR);
	exit(1);
	break;
      default:
	usage();
      }
    }
  
  if(optind<argc)
  {
    d = atof (argv[optind]);
    gotd = 1;
  }
  
  if(select_method==1) {
    /* don't wait for click */
    target_win = Get_Window_Under_Cursor(dpy);
  } else {
    /* grab mouse and return window that is next clicked */
    target_win = Select_Window(dpy);
  }
  
  unsigned char *data;

  Atom actual;
  int format;
  unsigned long n, left;


  if (gotd)
    opacity = (unsigned int) (d * OPAQUE);
  else 
    opacity = 0xc0000000;

  /* get property */
  XGetWindowProperty(dpy, target_win, XInternAtom(dpy, OPACITY, False), 
		     0L, 1L, False, XA_CARDINAL, &actual, &format, &n, &left, 
		     (unsigned char **) &data);
  
  if (data != None)
    {
      memcpy (&current_opacity, data, sizeof (unsigned int));
      XFree(( void *) data );
      //      printf("Found property: %g\n", (double) opacity / OPAQUE);
    }
  else
    current_opacity = OPAQUE;

  /* for user-compability with transset */
  if(!gotd) flag_toggle=1;
  
  /* toggle */ 
  if(flag_toggle)
    if (current_opacity != OPAQUE)
      opacity = OPAQUE;

  //  printf ("opacity 0x%x\n", opacity);
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
