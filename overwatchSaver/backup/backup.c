
#include "../includes/overwatch.h"
#include <stdio.h>
#include "../includes/pgsqldb.h"
#include "../includes/backupSaver.h"

const char *confLocation = "../info/database.conf";





   


int main( int argc, char *argv[])
{
checkBackupLog(confLocation);

return 0;

}
