//#CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//#searches for users as described in spec.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "SearchUsers.h"

void Search_Users(){
  sqlite3 *db; char *zErrMsg = 0;
  const char* data = "Callback function called\n";
  
  int choice;
  char s[50];
  char text[500];
  printf("Enter Keywords of User\n");
  int rc = sqlite3_open("test.db", &db);
  fgets(s,50,stdin);
  fgets(s,50,stdin);
  if(s[strlen(s)-1]=='\n')
    s[strlen(s)-1]='\0';
  //SELECT u.password FROM users u WHERE u.email = email
  char search[100];
  sprintf(search, "SELECT u.email, u.name, u.city FROM users u WHERE u.email LIKE \"%%%s%%\" OR u.name LIKE \"%%%s%%\"",s,s);

  printf("SQL: %s\n",search);
  rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

  if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
    fprintf(stdout, "Operation done successfully\n");
  }
  
  do{
    printf("1. Write a review\n2. List all active listings of user\n3. List all reviews of the user\n4. Back\n");
    scanf("%d",&choice);
    switch(choice){
    case 1:
      //get all needed info and insert into table
      printf("Enter email of user: ");
      scanf("%s",s);
      printf("Enter review text:\n");
      fgets(text,50,stdin);
      fgets(text,500,stdin);
      if(s[strlen(s)-1]=='\n')
	s[strlen(s)-1]='\0';
      printf("Enter rating: ");
      int rating=0;
      scanf("%d",&rating);
      //insert review statement NEED CURRENT USER VARIABLE
      sprintf(search, "insert into reviews values ('%s', '%s', %d, '%s', 'DATE('now')')",reviewer,s,rating,text);
      printf("SQL: %s\n",search);
      rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

      if( rc != SQLITE_OK )
	{
	  fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	}else{
	fprintf(stdout, "Operation done successfully\n");
      }
      
      break;
      //write review
    case 2:
      break;
      //list all active listings
    case 3:
      break;
      //list all reviews of the user
    case 4:
      return;
      //go back to previous menu
    default:
      printf("Error: Choice is not an option\n");
      return;
    }
  }while(1);
  sqlite3_close(db);
  return;
}

//example
//char* SQL_pwFetch = strcat("SELECT u.password FROM users u WHERE u.email = email", email);
//sqlite3_exec(db, SQL_pwFetch,callback,(void*)data, &zErrMsg);


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
