#include <stdio.h>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char **argv)
{
  MYSQL mysql;
  MYSQL_RES *msRes;
  MYSQL_ROW msRow;
  struct timeval pTime;
  unsigned int num_fields;
  unsigned int num_rows;
  int use_count;
  int mID;
  int i;
  unsigned long *lengths;
  char szrecord[1024];
  char *szusername;
  char szmodulename[255];

  
  if (argc < 2)
    {
      fprintf(stderr, "usage: %s <modulefile>\n", argv[0]);
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
      
  szusername=getenv("USER");
  if (szusername == NULL)
    {
      mysql_close(&mysql);
      fprintf(stderr, "Unable to read username\n");
      return -3;
    }
      
  if (gettimeofday(&pTime, NULL) != 0)
    {
      mysql_close(&mysql);
      fprintf(stderr, "Unable to get time of day\n");
      return -4;
    }
      
  sprintf(szrecord, "SELECT * FROM module_usage WHERE uid='%s' AND module_name='%s'", szusername, argv[1]);
  if(mysql_query(&mysql,szrecord)!=0)
    {
      fprintf(stderr, "Failed to search records: Error: %s\n", mysql_error(&mysql));
      mysql_close(&mysql);
      return -5;
    }
      
  msRes = mysql_use_result(&mysql);
  if (msRes != NULL)  // there are rows
    {
      num_fields = mysql_num_fields(msRes);
      use_count=0;
      while ((msRow = mysql_fetch_row(msRes)) != NULL)
	{
	  lengths = mysql_fetch_lengths(msRes);
	  for(i = 0; i < num_fields; i++)
	    {
	      mID=atoi(msRow[0]);
	      use_count=atoi(msRow[3]);
	    }
	}
      if (use_count > 0)
	{
	  use_count++;
	  mysql_free_result(msRes);
	  sprintf(szrecord, "UPDATE module_usage SET use_count=%d,last_used=%d WHERE ID=%d", use_count,
		  pTime.tv_sec, mID);
	  if(mysql_query(&mysql,szrecord)!=0)
	    {
	      mysql_close(&mysql);
	      fprintf(stderr, "Failed to update records: Error: %s\n", mysql_error(&mysql));
	      return -1;
	    }
	}
      else
	{
	  use_count=1;
	  sprintf(szrecord, "INSERT into module_usage VALUES('', '%s', '%s', %d, %d)", szusername, argv[1],
		  use_count, pTime.tv_sec);
	  if(mysql_query(&mysql,szrecord)!=0)
	    {
	      mysql_close(&mysql);
	      fprintf(stderr, "Failed to update records: Error: %s\n", mysql_error(&mysql));
	      return -1;
	    }
	}
    }
  else  // mysql_store_result() returned nothing; should it have?
    {
      if(mysql_field_count(&mysql) == 0)
	{
	  // query does not return data
	  // (it was not a SELECT)
	  num_rows = mysql_affected_rows(&mysql);
	}
      else // mysql_store_result() should have returned data
	{
	  fprintf(stderr, "Error: %s\n", mysql_error(&mysql));
	}
    }
  mysql_close(&mysql);
  return 0;
}
