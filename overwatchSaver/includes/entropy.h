/**
* @file entropy.h
* @brief Entropy tools
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 01/2017
*
*/


#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define bool int /* boolean tool */
#define true 1 /* boolean true */
#define false 0 /* boolean false */
#define STRING_SIZE 100 /* String size macro*/

#define STRING_MACRO(name) #name /* String tool */
#define BINWALK_ENTROPY STRING_MACRO(binwalk -E -N ) /* Binwalk command */
#define LS STRING_MACRO(ls) /* ls command */


/**
* @brief Calculate the entropy of a file 
*
* @param filePath: Path to the file target
*
* @return float, entropy | < 0 if any error occurred
* 
*/
float fileEntropyBinwalk(char * filePath);

/**
* @brief Calculate the entropy of a directory
*
* @param directoryPath: Path to the directory target
* @param fileOutput: Path to the output file
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int directoryEntropyBinwalk(char * directoryPath, char * fileOutput);

/**
* @brief Execute a command in the linux shell
*
* @param command: Comman to be executed
* 
*/
void command(char* command);

/**
* @brief Check if the path given is a directory
*
* @param dirPath: Path to the directory target
*
* @return bool (integer), 0 false  | 1 true | < 0 if any error occurred
* 
*/
bool isDirectory(char *dirPath);

/**
* @brief Append two parts of a path
*
* @param steem: First part of the path
* @param sufixDest: Second part and the final path target
*
* 
*/
void addPath(char *stemm, char *sufixDest);

/**
* @brief Delete double spaces from a string
*
* @param src: Source string
* @param dst: Target string
*
* 
*/
void deleteSpaces(char src[], char dst[]);

