#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

extern sqlite3* db;

int searchSales();
static int callback(void *data, int argc, char **argv, char **aColName);