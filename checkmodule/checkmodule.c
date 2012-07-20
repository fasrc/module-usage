#include <stdio.h>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

extern int processcommand(int argc, char *argv[], 
			  int *border, int *useexact, int *usecsv, 
			  char *szusername, char *szmodulename);


int main(int argc, char *argv[])
{
  MYSQL mysql;
  MYSQL_RES *msRes;
  MYSQL_ROW msRow;
  time_t pTime;
  int border=-1;
  int useexact=-1;
  int use_count;
  int usecsv=-1;
  int mID;
  int i;
  unsigned int num_fields;
  unsigned int num_rows;
  unsigned long *lengths;
  char szrecord[1024];
  char szusername[128];
  char szmodulename[128];
  char *szname;
  char *szmname;
  char szTime[256];


  if (processcommand(argc, argv, &border, &useexact, &usecsv, szusername, szmodulename) != 0)
    {
      fprintf(stderr, "Unable to process commandline\n");
      return -1;
    }
  if(mysql_init(&mysql)==NULL)
    {
      fprintf(stderr, "\nFailed to initate MySQL connection");
      return -1;
    }

  if (!mysql_real_connect(&mysql,"hulsmysql","rcg_module_usage","REDACTED",NULL/*variation #2*/,0,NULL,0)) 
    {
      fprintf(stderr, "Failed to connect to MySQL: Error: %s\n", mysql_error(&mysql));
      return -2;
    }
      
  if(mysql_select_db(&mysql,"rcg_module_usage" /*const char *db*/)!=0)
    {
      fprintf(stderr, "Failed to connect to Database: Error: %s\n", mysql_error(&mysql));
      return -2;
    }
     
  sprintf(szrecord, "SELECT * FROM module_usage");
  if (szusername[0] != '\0')
    {
      sprintf(szrecord, "%s WHERE uid='%s'", szrecord, szusername);
      if (szmodulename[0] != '\0')
	{
	  if (useexact < 0)
	    sprintf(szrecord, "%s AND module_name LIKE '%%%s%%'", szrecord, szmodulename);
	  else
	    sprintf(szrecord, "%s AND module_name='%s'", szrecord, szmodulename);
	}
    }
  else if (szmodulename[0] != '\0')
    {
      if (useexact < 0)
        sprintf(szrecord, "%s WHERE module_name LIKE '%%%s%%'", szrecord, szmodulename);
      else
        sprintf(szrecord, "%s WHERE module_name='%s'", szrecord, szmodulename);
    }
  sprintf(szrecord, "%s ORDER BY last_used DESC", szrecord);
  /*sprintf(szrecord, "%s ORDER BY use_count DESC", szrecord);*/
  if (usecsv < 0)
    {
      fprintf(stdout, "%s\n", szrecord);
    }
  if(mysql_query(&mysql,szrecord)!=0)
    {
      fprintf(stderr, "Failed to search records: Error: %s\n", mysql_error(&mysql));
      mysql_close(&mysql);
      return -5;
    }
      
  msRes = mysql_use_result(&mysql);
  if (msRes != NULL)  // there are rows
    {
      if (usecsv > 0)
        {
          fprintf(stdout, "%s,%s,%s,%s\n", "Name", "Module", "Count", "Time");
        }
      else
        {
          fprintf(stdout, "%13s%49s%15s%32s\n", "Name", "Module", "Count", "Time");
        }
      num_fields = mysql_num_fields(msRes);
      use_count=0;
      while ((msRow = mysql_fetch_row(msRes)) != NULL)
	{
	  lengths = mysql_fetch_lengths(msRes);
	  for(i = 0; i < num_fields; i++)
	    {
	      mID=atoi(msRow[0]);
	      szname=msRow[1];
	      szmname=msRow[2];
	      use_count=atoi(msRow[3]);
	      pTime=atoi(msRow[4]);
	    }
	  memcpy(szTime, ctime(&pTime), (size_t)(255*sizeof(char)));
	  if (usecsv > 0)
            {
	      fprintf(stdout, "%s,%s,%d,%s", szname, szmname, use_count, szTime);
	    }
	  else
            {
	      fprintf(stdout, "%13s%49s%15d%32s", szname, szmname, use_count, szTime);
	    }
	}
    }

  mysql_close(&mysql);
  return 0;
}
