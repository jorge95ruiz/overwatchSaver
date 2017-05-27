
#include "../includes/overwatch.h"
#include <stdio.h>
#include "../includes/pgsqldb.h"
#include "../includes/backupSaver.h"
#include "../includes/entropy.h"


/*************************************
*     Installing (libraries needed):
*
* apt-get install libpq-dev
* apt-get install postgresql
* apt-get install python-dev libfuzzy-dev
* apt-get install binwalk
*
*sudo sed -i -e 's/peer/trust/g' /etc/postgresql/9.5/main/pg_hba.conf 


* Guide:
* http://zetcode.com/db/postgresqlc/
*
*
*************************************/

// Create user from bash:
// sudo -u postgres bash -c "psql -c \"CREATE USER test WITH PASSWORD 'test';\""
// Create db from bash:
//sudo -u postgres createdb testdb --owner janbodnar




void converPrinter(){

int i =0;


for(i=0; i<80; i++){
    printf("$");
  }

  for(i=0; i<2; i++){
    printf("$                                                                              $");
  }
  
  
  printf("$            |I|I|I|I|I|              |I|                    |I|               $");
  printf("$            |I|       |I|             |I|                  |I|                $");
  printf("$            |I|       |I|              |I|       |I|      |I|                 $");
  printf("$            |I|I|I|I|I|                 |I|    |I||I|    |I|                  $");
  printf("$            |I|      |I|                 |I|  |I|  |I|  |I|                   $");
  printf("$            |I|       |I|                 |I||I|    |I||I|                    $");
  printf("$            |I|        |I|                 |I|        |I|                     $");
  printf("$                                                                              $");
  printf("$                                                                              $");
  printf("$                                                         Ransomwatcher by JR  $");
  



  for(i=0; i<80; i++){
    printf("$");
  }

  printf("\n\n");


}


   


int main( int argc, char *argv[] )
{
  int length, i = 0, choice;
  int fd;
  int wd;
  float f, entropy;
  char event[2000], path[200], filename[200];
  overwatched_processes processesList[PID_NUMBER];
  process_info * info;


lsofController("/home/jorge/Documentos/", 5, processesList, "../info/database.conf");
return 0;



}



