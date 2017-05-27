#include "../includes/fuzzyHash.h"



int fuzzyHashDirectory(char * directoryPath, FILE * fileOutput){

  
 if(directoryPath == NULL ){
    return -1;
   }

  if(fileOutput == NULL ){
    return -2;
   }


  if(isDirectory(directoryPath) == false){

     return -3;
    }

  FILE *fp;
  char ls[1035], command[1035], final[1035], def[1035];
  int i = 0;

  strcpy(command, LS);
  strcat(command, " ");
  strcat(command,directoryPath);


  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }


  while(fgets(ls, sizeof(ls)-1, fp)!= NULL){

    
    deleteSpaces(ls, final);
    addPath(directoryPath, final);
    deleteBarsDuplicated(final, def);
    
    printf("%s\n", def);
    if(isDirectory(def)){
        fuzzyHashDirectory(def, fileOutput);
    }else{
      fuzzyHashFromFile(def , fileOutput);
    }
  }


return 0;

}

int fuzzyHashFromFile(char *sourceFile , FILE * fileOutput){

  if(sourceFile == NULL){
    return -1;
  }

  if(fileOutput == NULL){
    return -2;
  }

  char result[1035];

  FILE *fp;

  fp = fopen(sourceFile, "r");

  if(fp == NULL){
    return-3;
  }

  fuzzy_hash_filename(fp, result);
  fprintf(fileOutput, "%s %s\n", sourceFile, result);
  fclose(fp);
  return 0;

}

int fuzzyHashCompare(char *fuzzyA , char * fuzzyB){

  if(fuzzyA == NULL || fuzzyB== NULL){
    return -1;
  }

  

  return fuzzy_compare(fuzzyA, fuzzyB);

}



