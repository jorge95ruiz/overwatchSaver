/**
* @file backupSaver.h
* @brief Utilities for creating a backup
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 03/2017
*
*/



#include <stdio.h>
#include <libpq-fe.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include "../includes/entropy.h"



#define CHAR_LENGTH 1035 /* String size macro*/
#define LITTLE_CHAR 30	/* String size macro*/
#define DAYS_CUTOFF 0   /* Numbers of days for updating a backup*/
#define ATTEMPS_NUM 10	/* Numbers of tries to connect to an usb*/



/**
* @brief Find out the path of a usb connected 
*
* @param path: Returns the path of the usb
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int takeUsbPath(char *path);

/**
* @brief Take the path of a usb connected 
*
* @param path: Returns the path of the usb
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int takeUsbInfo(char *path);

/**
* @brief Create a database over the oconfig parameters
*
* @param outfile: File where the database parameters are saved
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int createDataBase(char * outfile);

/**
* @brief Check if it is needed to make a backup or udate one.
*
* @param cong: File where the backup preferences are saved
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int checkBackupLog(char *conf);

/**
* @brief Create a full backup.
*
* @param usbPath: Path to store the backup
* @param path: Directory path for the backup
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int createFullBackup(char * usbPath, char * path);

/**
* @brief Create a zenity pop
*
* @param usbPath: Path to store the backup
* @param path: Directory path for the backup
*
* @return integer, 0 if user answer ok | 1 if user cancels | <1 if any error occurred
* 
*/
int zenityQuestionPopup(char * popupText);
