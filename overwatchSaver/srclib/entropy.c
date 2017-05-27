#include "../includes/entropy.h"



/*Calculate the entropy of each file of the directory
* given by using the binwalk shell command and output output it
* in the fileOutput
*
*/
int directoryEntropyBinwalk(char * directoryPath, char * fileOutput){
 if(directoryPath == NULL ){
    return -1;
  }


    if(isDirectory(directoryPath) == false){
      return -2;
    }
          

  FILE *fp;
  char ls[1035], command[1035], final[1035], def[1035];
  int i = 0;
  float entropy = 0;

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
        directoryEntropyBinwalk(def, fileOutput);
    }else{

      entropy = fileEntropyBinwalk(def);
    }
  }


return 0;

}


/*Calculate the entropy of a simple file given by using the binwalk shell command*/
float fileEntropyBinwalk(char * filePath){

  if(filePath == NULL){

    return -1;
  }
     

  FILE *fp;
  char path[1035], target[1035], command[1035];
  int i = 0;
  int targetIndex = 0;
  bool goOn = false;

  strcpy(command, BINWALK_ENTROPY);
  strcat(command, " ");
  strcat(command, filePath);
  strcat(command, " |grep \" F.*)\"");
 
 

  /* Open the command for reading. */
  fp = popen(command, "r");
  if (fp == NULL) {
    return -2;
  }

  while(fgets(path, sizeof(path)-1, fp)!= NULL){
  }
  
 
  
  for(i=0; i < strlen(path); i ++){
   
    if(goOn && path[i]!=41){
      target[targetIndex] = path[i];
      targetIndex++;
    }else{
      if(path[i] == 40){
        goOn = true;
       }
       else if (path[i]==41){
        goOn = false;
       }

    }
  }
  double entropy = atof(target);
  pclose(fp);

  return entropy;

  }

bool isDirectory(char *dirPath){
  if(dirPath ==NULL)
      return false;
    DIR *d;

    /*directorio actual*/
    if(strcmp(dirPath, " ")==0){
      return true;
    }

    struct dirent *dp;
    int dfd, ffd;
    if ((d = fdopendir((dfd = open(dirPath, O_RDONLY)))) == NULL) {
        return false;
    }
    return true;
}


void addPath(char *stemm, char *sufixDest){
    char final[1035];

    strcpy(final, stemm);
    strcat(final, "/");
    strcat(final, sufixDest);

    
    strcpy(sufixDest, final);
  

}

void deleteSpaces(char src[], char dst[]){


  int s, d=0;
  
  for (s=0; s< strlen(src); s++){
    if (src[s] != ' ' && src[s] != '\n') {
       dst[d] = src[s];
       d++;
    }
  }
       dst[d] = '\0';
}


void deleteBarsDuplicated(char src[], char dst[]){


  int s, d=0;
  
  for (s=0; s< strlen(src); s++){
    if (src[s] != '\n') {

      if( (s+1) < strlen(src) && src[s]=='/' && src[s+1]=='/'){

      }else{
        dst[d] = src[s];
       d++;
      }
       
    }
  }
       dst[d] = '\0';
}


