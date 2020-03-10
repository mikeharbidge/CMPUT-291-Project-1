#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "Login.h"
#include "SearchSales.h"
//#include "SearchUsers.h"

int main();

extern int openDB(char *filename, sqlite3 *db;);
//static int callback(void *data, int argc, char **argv, char **aColName); //Callback will NOT transfer between files
