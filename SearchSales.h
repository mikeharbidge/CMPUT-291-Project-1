#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

int searchSales();
static int callback(void *data, int argc, char **argv, char **aColName);