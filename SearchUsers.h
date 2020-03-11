//SearchUsers.h
//header file for searching users fuctionality
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void Search_Users();
static int callback(void *data, int argc, char **argv, char **aColName);
void write_review(char* userEmail, sqlite3* db);
