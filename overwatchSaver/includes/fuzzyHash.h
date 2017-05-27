/**
* @file fuzzyHash.h
* @brief FuzzyHash tools
*
* @author Jorge Antonio Ruiz Muñoz 
* @date 02/2017
*
*/


#include "entropy.h"
#include <stdio.h>
#include <fuzzy.h>


/**
* @brief Generate the Fuzzy hash of a directoy 
*
* @param directoryPath: Path to the target directory 
* @param fileOutput: File descriptor of the output file
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int fuzzyHashDirectory(char * directoryPath, FILE * fileOutput);

/**
* @brief Generate the Fuzzy hash of a file 
*
* @param sourceFile: Path to the target file 
* @param fileOutput: File descriptor of the output file
*
* @return integer, 0 if everything ok | < 0 if any error occurred
* 
*/
int fuzzyHashFromFile(char *sourceFile , FILE * fileOutput);

/**
* @brief Compare two  Fuzzy hash 
*
* @param fuzzyA: First Fuzzy hash code
* @param fuzzyB: Second Fuzzy hash code
*
* @return integer, % of similarity | < 0 if any error occurred
* 
*/
int fuzzyHashCompare(char *fuzzyA , char * fuzzyB);