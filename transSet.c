/* Simple program to toggle Translucency property
   Based on 'transset' by  Matthew Hawn
   With some additional features to make it more automatic and integrated
   The purpos is to bind it from your wm to a key or mouse-button

   Use however you want.
   Written by: Daniel Forchheimer (upiom)
   
*/

#define VERSIONSTR "3"

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
	  "    -n, --name NAME      select by name\n");
  fprintf(stderr,
	  "    -i, --id             select by window id\n");
  fprintf(stderr,
	  "        --inc            increase by the given opacity\n");
  fprintf(stderr,
	  "        --dec            decrease by given opacity\n");
  fprintf(stderr,
	  "    -m, --min OPACITY    minimum possible opacity (default = 0)\n");
  fprintf(stderr,
	  "    -x, --max OPACITY    maximum possible opacity (default = 1)\n");
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
  int flag_increase=0;
  int flag_decrease=0;
  int o;
  float min=0,max=1;
	char *idstr,*namestr;

	Window root_win;
	Window *child_list;
	unsigned int num_children;

  int options_index=0;
  static struct option long_options[] = {
    {"toggle",0,NULL,'t'},
    {"help",0,NULL,'h'},
    {"point",0,NULL,'p'},
    {"click",0,NULL,'c'},
    {"id",1,NULL,'i'},
    {"name",1,NULL,'n'},
    {"inc",0,NULL,'1'},
    {"dec",0,NULL,'2'},
    {"min",1,NULL,'m'},
    {"max",1,NULL,'x'},
    {"version",0,NULL,'v'},
    {0,0,0,0}
  };

  /* wonderful utility */
  Setup_Display_And_Screen(&argc, argv);

  while ((o = getopt_long(argc, argv, "thpci:n:vm:x:12",long_options,&options_index)) != -1)
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
      case 'i':
	idstr = malloc(strlen(optarg)+1);
	idstr = optarg;
	select_method=2;
	break;
      case 'n':
	namestr = malloc(strlen(optarg)+1);
	namestr = optarg;
	select_method=3;
	break;
      case '1':
	flag_increase=1;
	break;
      case '2':
	flag_decrease=1;
	break;
      case 'm':
	min = atof(optarg);
	break;
      case 'x':
	max = atof(optarg);
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
	} else if(select_method==2) {
    /* select by id, pretty much stripped from dsimple.c */
		sscanf(idstr, "0x%lx", &target_win);
		if (!target_win)
		  sscanf(idstr, "%ld", &target_win);
		if (!target_win) {
		  fprintf(stderr,"Invalid window id format: %s.\n", idstr);
			exit(0);
		}
		/* apparently we want the grandparent of this window, otherwise
		 * deleting the property won't work, cudos to another transset patch
		 * for leading me in this direction */
	  if (!XQueryTree(dpy, target_win, &root_win, &target_win, &child_list,
			  &num_children))
  	  Fatal_Error("Can't query window tree.");
  	if (!XQueryTree(dpy, target_win, &root_win, &target_win, &child_list,
			  &num_children))
  	  Fatal_Error("Can't query window tree.");
		
	} else if(select_method==3) {
    /* select by name, pretty much stripped from dsimple.c */
		target_win = Window_With_Name(dpy, RootWindow(dpy, screen),namestr);
		if (!target_win) {
			  fprintf(stderr,"No window with name %s exists!\n",namestr);
				exit(0);
		}
		/* get grandparent */
		if (!XQueryTree(dpy, target_win, &root_win, &target_win, &child_list,
			  &num_children))
  	  Fatal_Error("Can't query window tree.");
  	if (!XQueryTree(dpy, target_win, &root_win, &target_win, &child_list,
			  &num_children))
  	  Fatal_Error("Can't query window tree.");
		
  } else {
    /* grab mouse and return window that is next clicked */
    target_win = Select_Window(dpy);
  }
  
  unsigned char *data;

  Atom actual;
  int format;
  unsigned long n, left;

  if (!gotd) d=0.75;

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
  
  if(flag_increase) {
    d = (double)current_opacity/OPAQUE + d; 
  } else if(flag_decrease) {
    d = (double)current_opacity/OPAQUE - d;
  }
 //printf("%f\n",d);
  /* check min and max */
  if(d<min) d=min;
  if(d>max) d=max;

  opacity = (unsigned int) (d * OPAQUE);

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
