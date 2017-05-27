/**
* @file overwatch.h
* @brief Utilities to overwatch process and directories
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 04/2017
*
*/
#include "entropy.h"
#include <pthread.h>
#include "pgsqldb.h"


#define PID_NUMBER 32768 /* Max process pid */
#define ALPHA  10
#define ENTROPY_CUTOFF 1  /* entropy cutoff */
#define FUZZY_CUTOFF 0.01 /* Fuzzy hash cutoff*/

/**
 * @struct overwatched_processes
 * @brief Struct to save the process which being overwatched
 */
typedef struct 
  {
      int exist; 
  } overwatched_processes;

/**
 * @struct process_info
 * @brief Struct to save the info of a process
 */
  typedef struct 
  {	  char * path;
      int pid; 
      PGconn * conn;
      char * tableName;
  }process_info;


/**
* @brief remove a process in a secure way 
*
* @param pid: Pid of the target process
*
* 
*/
void removeRansom(int pid);

/**
* @brief take the name of the current linux user 
*
* @param outputName: String to save the name
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int whoami(char * outputName);

/**
* @brief Turn  all the interfaces
*
* @param mode: 0 turn down | turn up
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int turnInterfaces(int mode);

/**
* @brief Kill a process 
*
* @param pid: Pid of the target process
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int killProcess(int pid);

/**
* @brief Umoun all the disk of the system 
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int umountAll();

/**
* @brief Changue the buetooth servide state
*
* @param mode: 0 stop| start
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int turnBluethoot(int mode);

/**
* @brief Control a process
*
* @param information: Info of the process
*
* @return integer, 0 if everything ok | < 0 if any error occurred
*
*/
void * processMonitoring(process_info * information);


/**
* @brief Overwatch a process
*
* @param pid: Pid of the process
* @param path: Path of the directoy
* @param processesList: List of the curret overwatched process
* @param confFile:  database configuration file
*
* @return integer, 0 if everything ok | < 0 if any error occurred
*
*/
void processOverwatch(int pid, char *path, overwatched_processes * processesList, char * confFile);

/**
* @brief Overwatch a directory
*
* @param path: Path of the directoy
* @param seconds: Number of seceonds between each lsof call
* @param processesList: List of the curret overwatched process
* @param confFile:  database configuration file
*
*/
int lsofController(char * path, int seconds, overwatched_processes * processesList, char * confFile);

/**
* @brief generate entropy and FuzzyHash info of a directory
* and save it into a database
*
* @param dirPath: Path of the directoy
* @param config:  database configuration file
*
* @return integer, 0 if everything ok | < 0 if any error occurred
*
*/
int generateMasterInfo(char * dirPath, char * config);

/**
* @brief generate entropy and FuzzyHash of a directory and insert it
* into a database
* 
*
* @param dirPath: Path of the directoy
* @param connection:  database connection link
* @param tableName: Name of the target database table
*
* @return integer, 0 if everything ok | < 0 if any error occurred
*
*/
int infoDirectory(char * directoryPath, PGconn * connection, char * tableName);

/**
* @brief generate entropy and FuzzyHash of a file and insert it
* into a database
* 
*
* @param sourceFile: Path of the file
* @param connection:  database connection link
* @param tableName: Name of the target database table
*
* @return integer, 0 if everything ok | < 0 if any error occurred
*
*/
int infoDirectoryFile(char *sourceFile , PGconn * connection, char * tableName);

/**
* @brief Delete the first character of a string
*
* @param src: Source string
* @param dst: Target string
*
* 
*/
void deleteFirstCharacter(char src[], char dst[]);

/**
* @brief Calculate the ransomware value 
*
* @param entropy: Entropy value
* @param fuzzyHash: Fuzzy hash value
*
* @return float, value of the ransomware value 
*
*/
float calculateRansomValue(float entropy, float fuzzyHash);
