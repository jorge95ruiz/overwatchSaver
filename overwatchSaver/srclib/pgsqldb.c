#include "../includes/pgsqldb.h"


int recoverDBdataFromDb(PGconn * connection, char * tableName, char * type, char * date){

  if(connection == NULL || tableName == NULL || type == NULL || date == NULL){
    return -1;
  }


  char query[CHAR_LENGTH];

  strcpy(query, "SELECT date FROM ");
  strcat(query, tableName);
  strcat(query, " WHERE path ='");
  strcat(query, type);
  strcat(query, "'");



  PGresult *res = PQexec(connection,query);


  if (PQresultStatus(res) != PGRES_TUPLES_OK) {    
    PQclear(res);
    return -2;
    }

  if(res == NULL){
    return -3;
  }


  if(PQntuples(res) != 1){
    return -4;
  }

  strcpy(date, PQgetvalue(res, 0, 0));
  PQclear(res);

  return 0;
}

int recoverFiledataFromDb(PGconn * connection, char * tableName, char * fileName, float *entropy, char * fuzzyHash){

  if(connection == NULL || tableName == NULL || fileName == NULL || entropy == NULL || fuzzyHash == NULL){
    return -1;
  }


  char query[CHAR_LENGTH];

  strcpy(query, "SELECT entropy, fuzzyhash FROM ");
  strcat(query, tableName);
  strcat(query, " WHERE path ='");
  strcat(query, fileName);
  strcat(query, "'");



  PGresult *res = PQexec(connection,query);


  if (PQresultStatus(res) != PGRES_TUPLES_OK) {       
      PQclear(res);
      return -2;
  }

  if(res == NULL){
    return -3;
  }

  if(PQntuples(res) != 1){
    return -4;
  }


  *entropy =  atof(PQgetvalue(res, 0, 0));
  strcpy(fuzzyHash, PQgetvalue(res, 0, 1));

  PQclear(res);

  return 0;
}



int insertIntoTable(PGconn * connection, char * tableName, char * values){

  if(connection == NULL || tableName == NULL || values == NULL){
    return -1;
  }

  char query[CHAR_LENGTH];

  strcpy(query, "INSERT INTO ");
  strcat(query, tableName);
  strcat(query, " VALUES");
  strcat(query, values);
  

  PGresult *res = PQexec(connection, query);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      return -2;
      PQclear(res);

  }

  PQclear(res);


  return 0;



}
int createTable(PGconn * connection, char * tableName, char * arg){

  if(connection == NULL || tableName == NULL){
    return -1;
  }

  char query[CHAR_LENGTH];

  strcpy(query, "CREATE TABLE ");
  strcat(query, tableName);
  

  if(arg != NULL){
    strcat(query, arg);
  }else{
    strcat(query, "(Id INTEGER PRIMARY KEY)");
  }

  printf("query : %s\n", query);
  PGresult *res = PQexec(connection, query);

  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
      printf("%d\n", PQresultStatus(res));
      return -2;
      PQclear(res);
  }

  PQclear(res);


  return 0;
}

int createDb(char * dbname, char * owner){

if(dbname == NULL || owner == NULL){
  return -1;
}

FILE *fp;
char command[CHAR_LENGTH];
strcpy(command, "sudo -u postgres createdb ");
strcat(command, dbname);
strcat(command," --owner ");
strcat(command, owner);

fp = popen(command, "r");
  
if (fp == NULL) {
     return -2;
}

fclose(fp);

return 0;
}





PGconn * connectDb(char * dbname, char * userName, char * password){

if(dbname == NULL || userName == NULL){
  return NULL;
}

PGconn *connection = PQsetdbLogin("localhost", NULL, NULL, NULL, dbname, userName, password);

if (PQstatus(connection) != CONNECTION_OK){
    printf("%s\n", PQerrorMessage(connection));
    return NULL;
}


return connection;
}






void startPSQL(){

  FILE *fp;

  fp = popen("service postgresql start", "r");

  if (fp == NULL) {
    
    exit(1);
  }

  fclose(fp);
}

void stopPSQL(){

  FILE *fp;

  fp = popen("service postgresql stop", "r");
  
  if (fp == NULL) {
    exit(1);
  }

  fclose(fp);
}

int createPSQLUser(char * user, char * password){

  FILE *fp;
  char  command[CHAR_LENGTH];

  if(user == NULL || password == NULL){
  return -1;
  }


  strcpy(command, "sudo -u postgres bash -c \"psql -c \\\"CREATE USER ");
  strcat(command, user);
  strcat(command," WITH PASSWORD '");
  strcat(command, password);
  strcat(command,"';\\\"\"");

  fp = popen(command, "r");

  if (fp == NULL) {
      return -2;
  }

  fclose(fp);
  return 0;
}