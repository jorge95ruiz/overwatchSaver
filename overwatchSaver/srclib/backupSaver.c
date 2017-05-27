#include "../includes/backupSaver.h"

/*Shell commands*/
const char *usbInfo = "cat /var/log/syslog | grep -E \"New USB device found, idVendor\" | tail -n1 | sed 's/=/,/g' | cut -d ',' -f 3,5";
const char *isOn = "lsusb | grep -w \"";
const char *sdb = "dmesg | grep ";
const char *usbPath = "df -h | grep ";
const char *lsblk =" lsblk --scsi|grep usb |tail -n1| cut -d \" \" -f 1";
const char *createTableBuArg = " (type VARCHAR(10), date VARCHAR(15) , path VARCHAR (200), PRIMARY KEY (path))";
const char * createTableMArg = " (path VARCHAR(200), entropy FLOAT , fuzzyHash VARCHAR (100), PRIMARY KEY (path))";


int takeUsbPath(char *path){

  FILE *fp;
  char out[CHAR_LENGTH], sdbString[LITTLE_CHAR], command[CHAR_LENGTH];


  if(path == NULL){
    return -1;
  }
  
  fp = popen(lsblk, "r");


  if (fp == NULL) {
    return -1;
  }


   if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }
  

  deleteSpaces(out, sdbString);
  memset(out, 0, sizeof out);
  pclose(fp);


  strcpy(command, usbPath);
  strcat(command, sdbString);
  strcat(command, " | cut -d '\%' -f 2");

  fp = popen(command, "r");

  if (fp == NULL) {
    return -1;
  }

  if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }


  deleteSpaces(out, path);
  pclose(fp);

  


  return 0;

}

int takeUsbInfo(char *path){

  FILE *fp;
  char out[LITTLE_CHAR],  idVendor[LITTLE_CHAR], idProduct[LITTLE_CHAR], command[CHAR_LENGTH], aux[LITTLE_CHAR], sdbString[LITTLE_CHAR];
  char *pt;

  if(path == NULL){
    return -1;
  }
  
  fp = popen(usbInfo, "r");


  if (fp == NULL) {
    return -1;
  }

  if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }
  
  pclose(fp);


  strcpy(idVendor,strtok (out,","));
  strcpy(aux,strtok(NULL, ","));
  deleteSpaces(aux, idProduct);

  strcpy(command, isOn);
  strcat(command, idVendor);
  strcat(command, ":");
  strcat(command, idProduct);
  strcat(command, "\"");

  fp = popen(command, "r");

  if (fp == NULL) {
    return -1;
  }

   memset(out, 0, sizeof out);

  if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }

  memset(out, 0, sizeof out);
  memset(command, 0, sizeof command);
  pclose(fp);



  strcpy(command, sdb);
  strcat(command, idVendor);
  strcat(command, " -A 13 | grep ");
  strcat(command, idProduct);
  strcat(command, " -A 13 | tail -n1 | cut -d ':' -f 2");


  fp = popen(command, "r");

  if (fp == NULL) {
    return -1;
  }

   memset(out, 0, sizeof out);

  if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }

  deleteSpaces(out, sdbString);
  memset(out, 0, sizeof out);
  memset(command, 0, sizeof command);
  pclose(fp);




  strcpy(command, usbPath);
  strcat(command, sdbString);
  strcat(command, " | cut -d '\%' -f 2");


  fp = popen(command, "r");

  if (fp == NULL) {
    return -1;
  }

  if(fgets(out, sizeof(out)-1, fp)== NULL){
    return -1;
  }


  deleteSpaces(out, path);
  pclose(fp);

  return 0;
}

int createDataBase(char * outfile){

  if(outfile == NULL)
      return -1;



    char user[STRING_SIZE], userPassword[STRING_SIZE], dbname[STRING_SIZE], tableName[STRING_SIZE], tableNameMonitor[STRING_SIZE];
    int createUser, createDB, infoDir;
    FILE *fp;

    fp = fopen(outfile, "w");

    if(fp == NULL){
      return -2;
    }


    startPSQL();
    strcpy(user, "overwatchUser");
    strcpy(userPassword, "overwatchPassword");
    strcpy(dbname, "overwatch");
    strcpy(tableName, "backupdata");
    strcpy(tableNameMonitor, "monitor");

    while(createUser = createPSQLUser(user, userPassword) != 0){

      if(createUser == -1){
        return -3;
      }

      strcat(user, "1");
    }
    
  


    while(createDB = createDb(dbname, user) != 0){

      if(createDB == -1){
        return -4;
      }

      strcat(dbname, "1");
    }

    

   PGconn * conn =  connectDb(dbname, user, userPassword);

   if(PQstatus(conn) != CONNECTION_OK){
      return -5;
   }

   if(createTable(conn, tableName, createTableBuArg) != 0){
    return -7;
   } 

   if(createTable(conn, tableNameMonitor, createTableMArg) != 0){
    return -7;
   } 

   stopPSQL();

   fprintf(fp, "%s %s %s %s %s\n", dbname, tableName, tableNameMonitor, user, userPassword);
   fclose(fp);
   return 0;

}


int checkBackupLog(char *conf){

FILE *fp, *fc;

if(conf == NULL){
  return -1;
}

int out = 0, date[4], i=0, checker = 0, attemps=0;
char path[CHAR_LENGTH], buOutput[CHAR_LENGTH], values[CHAR_LENGTH], dateAux[LITTLE_CHAR], delete[CHAR_LENGTH];
char logMode[LITTLE_CHAR],  logDate[LITTLE_CHAR], logDir[LITTLE_CHAR],  logFilename[LITTLE_CHAR], usbPath[STRING_SIZE];
char dbName[LITTLE_CHAR], tableName[LITTLE_CHAR], username[LITTLE_CHAR], userPassword[LITTLE_CHAR], option[LITTLE_CHAR];
char *pt, dbPath[CHAR_LENGTH], daysCutoff[LITTLE_CHAR], auxiliar[LITTLE_CHAR];
PGconn * conn;

time_t t;
struct tm tm;
fc = fopen("../info/conf.txt", "r");

if(fc == NULL){
  return -2;
}



if(fscanf(fc, "%s %s", path, daysCutoff) != 2){
  return -3;
}
fclose(fc);


fp = fopen(conf, "r");

if(fp == NULL){
  return -2;
}


if (fscanf(fp, "%s %s %s %s %s", dbName, tableName, auxiliar, username, userPassword) != 5){
  return -6;
}


  conn =  connectDb(dbName, username, userPassword);

   if(PQstatus(conn) != CONNECTION_OK){
      return -7;
   }

 

if(recoverDBdataFromDb(conn, tableName, path, logDate) != 0){


  while(takeUsbPath(usbPath) != 0 && attemps < ATTEMPS_NUM){
   
    attemps++;
    sleep(20);
   }

   if(attemps == ATTEMPS_NUM)
   {

    fc = fopen("../log/log.txt", "a");

    if(fc == NULL){
      return -2;
     }

    fprintf(fc, "backup attempt failed usb not found\n");
    fclose(fc);
    return -8;
   }
   
  if (createFullBackup(usbPath, path)!= 0)
  {
    return -5;
  }
  t = time(NULL);
  tm = *localtime(&t);

  sprintf(logDate, "%d_%d_%d",  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);



  strcpy(values, "('full', '");
  strcat(values, logDate);
  strcat(values, "', '");
  strcat(values, path);
  strcat(values, "')");


  insertIntoTable(conn, tableName, values);
  fc = fopen("../log/log.txt", "a");

  if(fc == NULL){
  return -2;
  }

  fprintf(fc, "new backup %s of %s  stored in: %s\n",logDate, path,  usbPath);
  PQfinish(conn);
  fclose(fc);

return 0;


}/*there is a previous backup in the db*/
else{

  const char tok[2] = "_";
  
  strcpy(dateAux, logDate);
  strcat(dateAux, "_");
  pt = strtok (dateAux,tok);


    while (pt != NULL && i < 3) {
        date[i] = atoi(pt);
        pt = strtok (NULL, tok);
        i++;
    }
   


  t = time(NULL);
  tm = *localtime(&t);
  char message[CHAR_LENGTH];
  int daysDiferrence=0;

  
  daysDiferrence =  (tm.tm_year + 1900 - date[0]) * 365 +  (tm.tm_mon + 1 -date[1]) * 30 + tm.tm_mday -date[2]; 

  /*Necessary backup*/
  if(daysDiferrence >= atoi(daysCutoff)){

    
      sprintf(logDate, "%d_%d_%d",  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

       
        

      while(takeUsbPath(usbPath) != 0 && attemps < ATTEMPS_NUM){
         
         attemps++;
         sleep(20);
        }

       
      if(attemps == ATTEMPS_NUM)
      {

        fc = fopen("../log/log.txt", "a");

        if(fc == NULL){
          return -2;
         }

        fprintf(fc, "backup attempt failed usb not found %s\n", logDate);
        fclose(fc);
      }

      if(createFullBackup(usbPath,path)!= 0)
      {
          return -5;
      }

        conn =  connectDb(dbName, username, userPassword);

        strcpy(delete, "DELETE FROM ");
        strcat(delete, tableName);
        strcat(delete, " where path = '");
        strcat(delete, path);
        strcat(delete, "'");

        PGresult *res = PQexec(conn, delete);
        PQclear(res);   
        


        strcpy(values, "('full', '");
        strcat(values, logDate);
        strcat(values, "a', '");
        strcat(values, path);
        strcat(values, "')");

       insertIntoTable(conn, tableName,  values);
       PQfinish(conn);
       fc = fopen("../log/log.txt", "a");

       if(fc == NULL){
        return -2;
       }

      fprintf(fc, "backup updated %s of %s  stored in: %s\n",logDate, path,  usbPath);
      fclose(fc);
  }
}

return 0;

}





//tar -cpvzf "fullbackup_`date +%d%m%Y`.tgz" /home/jorge/Android/
int createFullBackup(char * usbPath, char * path){

  if(path == NULL || usbPath == NULL){
    return -1;
  }
  
  char command[CHAR_LENGTH], date[LITTLE_CHAR], filename[LITTLE_CHAR];
  FILE *fp;

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  //printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);


  sprintf(date, "%d_%d_%d",  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  strcpy(filename, "/fullbp_");
  strcat(filename, date);
  strcat(filename, ".tgz");


  strcpy(command, "tar -cpvzf ");
  strcat(command, usbPath);
  strcat(command, filename);
  strcat(command, " ");
  strcat(command, path);
  strcat(command, " >> /dev/null");
  
    fp = popen(command, "r");


    if (fp == NULL) {
      return -2;
    }

 
  
  pclose(fp);
  return 0;

}

 



/*-1 bad string -2 bad popen 0 ok or closing 1 canceling*/
int zenityQuestionPopup(char * popupText){

  if(popupText == NULL){
    return -1;
  }

  int answer;
  FILE  *fp;
  char command[CHAR_LENGTH], ls[CHAR_LENGTH];


  strcpy(command, "zenity --question --text \""); 
  strcat(command, popupText);
  strcat(command, "\"; echo $? >> /dev/null");


 fp = popen(command, "r");


  if (fp == NULL) {
    return -2;
  }

   

  if(fgets(ls, sizeof(ls)-1, fp)!= NULL){
     answer = atoi(ls);
  }

  return answer;
}
   
