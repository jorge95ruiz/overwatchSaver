

#include "../includes/overwatch.h"


void removeRansom(int pid){

  killProcess(pid);
  turnInterfaces(0);
  umountAll();
  turnBluethoot(0);

  FILE * fp;
  fp = fopen("../log/monitor.log", "a");

  if(fp == NULL){
    return ;
  }

  fprintf(fp, "Process %d\n removed", pid);
  fclose(fp);
}

    /*1 up 0 down*/
int turnInterfaces(int mode){

  if(mode < 0 || mode > 1){
    return -1;
  }

  char command[CHAR_LENGTH];
  FILE *fp;

  if(mode){
    strcpy(command, "sudo ifup -a");
  }else {
    strcpy(command, "sudo ifdown -a");
  }
  
 

  fp = popen(command, "r");


  if (fp == NULL) {
      return -2;
  }

  pclose(fp);
  return 0;

}

/*1 up 0 down*/
int turnBluethoot(int mode){

  if(mode < 0 || mode > 1){
    return -1;
  }

  char command[CHAR_LENGTH];
  FILE *fp;

  if(mode){
    strcpy(command, "sudo  /etc/init.d/bluetooth start");
  }else {
    strcpy(command, "sudo  /etc/init.d/bluetooth stop");
  }
  
 

  fp = popen(command, "r");


  if (fp == NULL) {
    return -2;
  }

  pclose(fp);
  return 0;

}



int killProcess(int pid){

  if(pid < 0){
    return -1;
  }

  char command[STRING_SIZE], pidChar[STRING_SIZE];
  FILE * fp;
  
  sprintf(pidChar, "%d", pid);

  strcpy(command, "kill -9 ");
  strcat(command, pidChar);

  fp = popen(command, "r");
  if (fp == NULL) {
    return -1;
  }

  pclose(fp);
  return 0;

}


int umountAll(){

  
  FILE * fp;
  
  fp = popen("sudo umount -a &", "r");
  if (fp == NULL) {
    return -1;
  }

  pclose(fp);
  return 0;

}

int whoami(char * outputName){

  
  if(outputName == NULL ){
    return -1;
  }

  char out[STRING_SIZE];
  FILE * fp;

  fp = popen("whoami", "r");
  if (fp == NULL) {
    exit(1);
  }


  while(fgets(out, sizeof(out)-1, fp)!= NULL){

    strcpy(outputName, out);
  }

  pclose(fp);
  return 0;

}

int infoDirectory(char * directoryPath, PGconn * connection, char * tableName){

  
 if(directoryPath == NULL || connection == NULL || tableName == NULL){
    return -1;
  }

  


  if(isDirectory(directoryPath) == false){
     return -2;
  }

  FILE *fp;
  char ls[CHAR_LENGTH], command[CHAR_LENGTH], final[CHAR_LENGTH], def[CHAR_LENGTH];
  int i = 0;

  strcpy(command, LS);
  strcat(command, " ");
  strcat(command,directoryPath);


  fp = popen(command, "r");
  if (fp == NULL) {
    return -3;
  }


  while(fgets(ls, sizeof(ls)-1, fp)!= NULL){ 
   
    deleteSpaces(ls, final);
    addPath(directoryPath, final);
    deleteBarsDuplicated(final, def);

    if(isDirectory(def)){
        infoDirectory(def, connection, tableName);
    }else{
      infoDirectoryFile(def , connection, tableName);
    }
  }


return 0;

}



int infoDirectoryFile(char *sourceFile , PGconn * connection, char * tableName){

  if(sourceFile == NULL || connection == NULL || tableName == NULL){
    return -1;
  }


  char fuzzyHash[CHAR_LENGTH], values[CHAR_LENGTH], entropyString[CHAR_LENGTH];
  FILE *fp;
  
  if(fileEntropyBinwalk(sourceFile) < 0){
    return 0;
  }

  sprintf(entropyString, "%g", fileEntropyBinwalk(sourceFile));


  fp = fopen(sourceFile, "r");
  if(fp == NULL){
    return -2;
  }
 

  if(fuzzy_hash_file(fp, fuzzyHash) !=  0){
    return -3;
  }
 
  fclose(fp);

  

  strcpy(values, "('");
  strcat(values, sourceFile);
  strcat(values, "', ");
  strcat(values, entropyString);
  strcat(values, " , '");
  strcat(values, fuzzyHash);
  strcat(values, "')");


  insertIntoTable(connection,tableName, values);
  
  return 0;

}

int generateMasterInfo(char * dirPath, char * config){

  if(dirPath == NULL || config == NULL)
      return -1;

  if(isDirectory(dirPath) == 0)
      return -2;

    char user[STRING_SIZE], userPassword[STRING_SIZE], dbname[STRING_SIZE], tableName[STRING_SIZE], aux[STRING_SIZE];
    int createUser, createDB, infoDir;
    FILE * fp;


    startPSQL();
    fp = fopen(config, "r");

    if(fp == NULL)
    {
    return -4;
    }

    if (fscanf(fp, "%s %s %s %s %s", dbname, aux, tableName, user, userPassword) != 5){
      return -6;
    }

    fclose(fp);



   PGconn * conn =  connectDb(dbname, user, userPassword);

   if(conn == NULL){
      return -5;
   }
    

   infoDir = infoDirectory(dirPath,conn, tableName);

   stopPSQL();
   return infoDir;

}


void * processMonitoring(process_info *information){

FILE *fp, *fc;
char command[CHAR_LENGTH], delete[CHAR_LENGTH], ls[CHAR_LENGTH],final[CHAR_LENGTH], file[CHAR_LENGTH], fuzzyHashMaster[CHAR_LENGTH], fuzzyHash[CHAR_LENGTH];
int round = 0, write = 0, fuzzy = 0,totalFuzzyHash=0;
float totalEntropy=0, entropy, entropyMaster;
PGresult * res;


  /*-p: process pid to overwatch*/
  strcpy(command, "lsof -p");
  sprintf(command , "%s %d ", command, information->pid);
  /*-a: multiple options +D: path to overwatch*/
  strcat(command, "-a +D ");
  strcat(command, information->path);
  /*-d : file descriptors to watch -Fan: display only acess mode and file path*/
  strcat(command, " -d w,1,3,u -Fan");
  
  
  while(1){

    if(calculateRansomValue(totalEntropy, totalFuzzyHash) >= ALPHA){
  
      removeRansom(information->pid);

    }
    



    

    round = 0;
    write = 0;
    fp = popen(command, "r");


    if (fp == NULL) {
      return -3;
    }



    while(fgets(final, sizeof(final)-1, fp)!= NULL){
       deleteSpaces(final, ls);
    
      switch(round) {

       case 0:
          round++;
          break; 
       case 1:
          round++;
          break; 
       case 2:
          /*If the process has writting acces (aw) or execution acces (au)*/
          if(strcmp(ls, "aw") == 0 || strcmp(ls, "au")){
              write = 1;
          }           
          round++;
          break;
        case 3:
          if(write){

            deleteFirstCharacter(ls, file);
            deleteSpaces(file, ls);
            entropy = fileEntropyBinwalk(ls);
            fc = fopen(ls, "r");
            if(fc = NULL){
             return -2;
             }
            fuzzy_hash_file(ls, fuzzyHash);
            fclose(fc);
            if(entropy == 0){
              write = 0;
              round = 0;
              break;
            }
            if(recoverFiledataFromDb(information->conn, information->tableName, ls, &entropyMaster, fuzzyHashMaster) == 0)
            {
              fuzzy = 100 -fuzzyHashCompare(fuzzyHashMaster, fuzzyHash);
              entropy = entropy - entropyMaster;
              if(entropy <0){
                entropy *= -1;
              }

              /*archivo modificado*/
              if(fuzzy != 0 || entropy != 0){
                totalFuzzyHash += fuzzy;
                totalEntropy += entropy;


                strcpy(delete, "DELETE FROM ");
                strcat(delete, information->tableName);
                strcat(delete, " where path = '");
                strcat(delete,ls);
                strcat(delete, "'");

                res = PQexec(information->conn, delete);
                PQclear(res);   
                /*insertDb new values*/
                infoDirectoryFile(ls , information->conn, information->tableName);
              }


            }

            
            
            write = 0;
          }
          round = 0;
          break;

      }


     }
    sleep(5);
  }
  free(information);
  return NULL;
}

void processOverwatch(int pid, char *path, overwatched_processes * processesList, char * confFile) {

  pthread_t thread_id;
  process_info * info;
  char user[STRING_SIZE], dbName[STRING_SIZE], tableName[STRING_SIZE], auxiliar[STRING_SIZE], userPassword[STRING_SIZE];
  FILE *fp;


  if(pid < 0 || processesList == NULL || confFile == NULL ){
    return ;
  }

  fp = fopen(confFile, "r");

  if(fp == NULL)
  {
    return ;
  }

  if (fscanf(fp, "%s %s %s %s %s", dbName, auxiliar, tableName, user, userPassword) != 5){
  return -6;
  }
  fclose(fp);
  info = malloc(sizeof( process_info));

  /*If the process is not being overwatched at this moment*/
  if(processesList[pid].exist != 1 && pid > 0){
        processesList[pid].exist = 1;
        info->path = path;
        info->pid = pid;

      
        PGconn * conn =  connectDb(dbName, user, userPassword);

        if(conn == NULL){
            return  ;
        }

        info->conn = conn;
        info->tableName = tableName;

        if (pthread_create(&thread_id, NULL, processMonitoring,  info) < 0) {
                perror ("-Server: Error creating thread");
                return ;
        }
  }
  
}


int lsofController(char * path, int seconds, overwatched_processes * processesList, char * confFile){

  FILE *fp;
  int pid;
  int test = 0;
  char command[STRING_SIZE],ls[CHAR_LENGTH];
  

  if(path == NULL || seconds < 0)
      return -1;

  /*Checking whether the path is a directory*/  
  if(isDirectory(path) == 0){
     return -2;
   }

  strcpy(command, "lsof -a +D ");
  strcat(command, path);
  strcat(command, " -d w,1,3,u -t");
  startPSQL();
  printf("%s\n", command);
    while(1){
    fp = popen(command, "r");


    if (fp == NULL) {
      return -3;
    }

   

    while(fgets(ls, sizeof(ls)-1, fp)!= NULL){
    
      if(sscanf(ls,"%d", &pid) != EOF && test == 0){
     
        processOverwatch(pid, path, processesList, confFile);
        test = 1;
       
      } 


     }
   sleep(seconds);
  }


  return 0;
}


void deleteFirstCharacter(char src[], char dst[]){


  int s;
  
  for (s=0; s< strlen(src); s++){
       dst[s] = src[s+1];
  }
 
  dst[s] = '\0';
}




float calculateRansomValue( float entropy, float fuzzyHash){
   entropy * ENTROPY_CUTOFF + fuzzyHash * FUZZY_CUTOFF;
};

