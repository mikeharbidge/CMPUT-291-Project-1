//PostSale.h
//header file for searching users fuctionality
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void Post_Sale(char* userEmail, sqlite3* db);
static int callback(void *data, int argc, char **argv, char **aColName);
