//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//main.c contains the main function of the program and calls all other functions
//Main also includes utility functions for opening and closing SQL databases
#include "main.h"

int main(){
    //testing for login
    //NEEDS TO ERROR CHECK DB INIT
    int in = 0; //if login succeeded
    int choice;//input for choosing functionality
    in = loginScreen();
    if (in){
        printf("***LOGIN SUCCEEDED***\n");
	printf("1. List Products\n2. Search For Sales\n3. Post a Sale\n4. Search for Users\n5. Logout\n");
	while(scanf("%d",&choice)){
	  switch(choice){
	  case 1:
	    printf("chose 1\n");
	    break;
	    //list products
	  case 2:
	    printf("chose 2\n");
	    break;
	    //Search for Sales 
	  case 3:
	    printf("chose 3\n");
	    break;
	    //Post a sale
	  case 4:
	    printf("chose 4\n");
	    break;
	    //Search for users
	  case 5:
	    printf("chose 5\n");
	    return 0;//temp
	    break;
	    //logout and close DB
	  default:
	    printf("Error: %d is not an option\n", choice);
	    break;
	  }
	}
    }
    return 0;
    }

extern int openDB(char *filename, sqlite3 *db)
{
    //wrapper function that opens a database connection with the given filename and sqlite3 object.
    //returns 0 if failed, 1 if succeeded
    //to close use standard sqlite3_close()
    int rc = sqlite3_open(filename, &db);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else 
    {
        fprintf(stderr, "Opened database successfully\n");
        return 0;
    }
    return 0;
}

static int callback(void *data, int argc, char **argv, char **aColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i = 0; i<argc; i++)
    {
    printf("%s = %s\n", aColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
