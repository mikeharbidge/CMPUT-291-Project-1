#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

//GLOBAL variables that can be used elsewhere
char userPwd[20];
char userEmail[20];

int loginScreen();
int signIn();
int signUp();