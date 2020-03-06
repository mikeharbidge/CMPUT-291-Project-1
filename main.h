#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "Login.h"

int main();

int openDB(char *filename, sqlite3 *db;)

static int callback(void *data, int argc, char **argv, char **aColName);