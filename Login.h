//this isnt really finished yet
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

int loginScreen();
int signIn();
int signUp();
static int callback(void *data, int argc, char **argv, char **aColName);