//SearchUsers.h
//header file for searching users fuctionality
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

void Search_Users();
static int callback(void *data, int argc, char **argv, char **aColName);
<<<<<<< HEAD
=======
void write_review(char* userEmail, sqlite3* db);
void list_listings(char* userEmail, sqlite3* db);
void list_reviews(char* userEmail, sqlite3* db);
>>>>>>> 695f0d476a5c40be2213f1fdad93e9dd1ad1858e
