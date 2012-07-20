/*
 *  commandline.c
 *  checkmodule
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * usage: Print out the usage information
 */
int usage(char *szCommand)
{
  fprintf(stdout, "Usage: %s -h or -m modulename -e -u username\n", szCommand);
  fprintf(stdout, "\t-h: Print this message\n");
  fprintf(stdout, "\t-m modulename: List the users of module sorted in decreasing order\n");
  fprintf(stdout, "\t-e: Use exact module name match\n");
  fprintf(stdout, "\t-c: Produce CSV output \n");
  fprintf(stdout, "\t-u username: List the usage of a single user\n");
  /* fprintf(stdout, "\t-r: Reverse the sort order\n");*/
  return 0;
}

/**
 * process the command line and parse out the arguments
 */
int processcommand(int argc, char *argv[], 
		   int *border,
		   int *useexact,
		   int *usecsv,
		   char *szusername,
		   char *szmodulename)
{
  int c;					/* command */
  extern char *optarg;
  extern int optind, optopt;	
  const int MINARGS=0;
  int nargs=0;
  int i;
	
  *border=-1;
  *useexact=-1;
  *usecsv=-1;
  szusername[0]='\0';
  szmodulename[0]='\0';
  while ((c = getopt(argc, argv, "chem:u:r")) != -1) 
    {
      switch (c) 
	{
	case 'h':
	  usage(argv[0]);
	  exit(0);
	  break;
				
	case 'm':
	  nargs++;
	  sprintf(szmodulename, "%s", optarg);
	  break;
			
	case 'e':
	  nargs++;
  	  *useexact=1;	  
	  break;

	case 'c':
	  nargs++;
  	  *usecsv=1;	  
	  break;
			
			
	case 'u':
	  nargs++;
	  sprintf(szusername, "%s", optarg);
	  break;
			
	case 'r':
	  nargs++;
	  *border=1;
	  break;
			
        case '?':
	  fprintf(stderr, "Unrecognised option: -%c\n", optopt);
	  usage(argv[0]);
	  exit(-1);
	  break;
			
	default:
	  usage(argv[0]);
	  exit(-1);
	  break;
	}
    }
  if (nargs < MINARGS)
    {
      fprintf(stderr, "%s: Incorrect arguments\nMissing required arguments\n", argv[0]);
      usage(argv[0]);
      exit(-1);
    }
  return 0;
}
