#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

//GLOBAL variables that can be used elsewhere
char userPwd[20];
char userEmail[20];
extern sqlite3* db;

int loginScreen();
int signIn();
int signUp();