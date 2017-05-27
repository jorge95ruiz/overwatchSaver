#include "../includes/md5.h"


int directoryMd5(char * directoryPath, FILE * fileOutput){

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
  char ls[1035], command[1035], final[1035];
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
 
    if(isDirectory(final)){
        directoryMd5(final, fileOutput);
    }else{
      md5FromFile(final , fileOutput);
    }
  }

return 0;

}

int md5FromFile(char *md5File , FILE * fileOutput){

	unsigned char c[MD5_DIGEST_LENGTH];
	int i;
    FILE *inFile = fopen(md5File, "rb");
    
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        return -1;
    }

    if (fileOutput == NULL) {
        return -2;
    }



    MD5_Init(&mdContext);


    while ((bytes = fread (data, 1, 1024, inFile)) != 0){
        MD5_Update(&mdContext, data, bytes);
    }

    MD5_Final(c,&mdContext);

    

    fprintf(fileOutput, "%s ", md5File);

    

    

    for(i = 0; i < MD5_DIGEST_LENGTH; i++){
    	 fprintf(fileOutput, "%02x", c[i]);
		}


    fprintf(fileOutput, "\n");

	
    

    fclose (inFile);
    return 0;




}



