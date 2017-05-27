/**
* @file pgsqldb.h
* @brief Utilities over a PostgreSql databse
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 01/2017
*
*/


#include <libpq-fe.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define CHAR_LENGTH 1035  /* String size macro*/


/**
* @brief Create a table in a PostgreSql database
*
* @param connection: Active connection to the database
* @param tableName: Table name
* @param arg: Arguments regarding the fields of the table
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int createTable(PGconn * connection, char * tableName, char * arg);

/**
* @brief Insert into a table in a PostgreSql database
*
* @param connection: Active connection to the database
* @param tableName: Table name
* @param values: Arguments regarding the fields of the table
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int insertIntoTable(PGconn * connection, char * tableName, char * values);

/**
* @brief Recover data from a table in a PostgreSql database
*
* @param connection: Active connection to the database
* @param tableName: Table name
* @param fileName: Name of the file which is goona be look for
* @param entropy: Entropy of the file given
* @param fuzzyHash: FuzzyHash code of the file given
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int recoverFiledataFromDb(PGconn * connection, char * tableName, char * fileName, float *entropy, char * fuzzyHash);

/**
* @brief Recover data from a table in a PostgreSql database
*
* @param connection: Active connection to the database
* @param tableName: Table name
* @param type: Name of the file which is goona be look for
* @param date: Date of the file given
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int recoverDBdataFromDb(PGconn * connection, char * tableName, char * type, char * date);

/**
* @brief Create  a PostgreSql database
*
* @param dbname: Database name
* @param owner: Name of the owner of the database
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int createDb(char * dbname, char * owner);

/**
* @brief Connect to a PostgreSql database
*
* @param dbname: Database name
* @param userName: Name of the user for the connection
* @param password: User´s password for the connection
*
* @return Pgconn with the database connection if everything ok | Null if any error occurred
* 
*/
PGconn * connectDb(char * dbname, char * userName, char * password);

/**
* @brief Create  a PostgreSql user
*
* @param user: User´s name
* @param password: User´s password
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int createPSQLUser(char * user, char * password);

/**
* @brief Activate  PostgreSql service
* 
*/
void startPSQL();

/**
* @brief Stopp  PostgreSql service
* 
*/
void stopPSQL();

