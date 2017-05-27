
#include "../../includes/overwatch.h"
#include <stdio.h>
#include "../../includes/pgsqldb.h"
#include "../../includes/backupSaver.h"

const char *confLocation = "../../info/database.conf";
const char *backupPath = "/home/jorge/Escritorio/";


   


int main( int argc, char *argv[])
{
createDataBase(confLocation);
generateMasterInfo(backupPath, confLocation);
return 0;

}
