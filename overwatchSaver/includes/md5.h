/**
* @file pgsqldb.h
* @brief Utilities over a PostgreSql databse
* Based on openssl/md5.h more info in:
* https://www.openssl.org/docs/man1.0.2/crypto/md5.html
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 01/2017
*
*/



#include "entropy.h"
#include <stdio.h>
#include <openssl/md5.h>

/**
* @brief Calculate the MD5 hash code of a directory
*
* @param directoryPath: Path to the directory
* @param File: file descriptor to save the hash
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int directoryMd5(char * directoryPath, FILE * fileOutput);


/**
* @brief Calculate the MD5 hash code of a file
*
* @param med5file: File path 
* @param fileOutput: file descriptor to save the hash
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int md5FromFile(char *md5File , FILE * fileOutput);