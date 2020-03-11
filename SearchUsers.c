//#CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//#searches for users as described in spec.
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "SearchUsers.h"

void Search_Users(char* userEmail,sqlite3* db){
  char *zErrMsg = 0;
  const char* data = "Callback function called";
  
  char choice;
  char s[50];
  printf("Enter Keyword of User: ");
  int rc;
  fgets(s,50,stdin);
  fgets(s,50,stdin);
  if(s[strlen(s)-1]=='\n')
    s[strlen(s)-1]='\0';
  //SELECT u.password FROM users u WHERE u.email = email
  char search[100];
  sprintf(search, "SELECT u.email, u.name, u.city FROM users u WHERE u.email LIKE \"%%%s%%\" OR u.name LIKE \"%%%s%%\"",s,s);

  //  printf("SQL: %s\n",search);
  rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

  if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
    fprintf(stdout, "Operation done successfully\n");
  }
  printf("1. Write a review\n2. List all active listings of user\n3. List all reviews of the user\n4. Back\nInput:\
 ");
  
  while(scanf("%c",&choice)){
    switch(choice){
    case '1':
      write_review(userEmail,db);     
      break;
      //write review
    case '2':
      list_listings(userEmail,db);
      break;
      //list all active listings
    case '3':
      list_reviews(userEmail,db);
      break;
      //list all reviews of the user
    case '4':
      printf("Returning...\n");
      return;
      //go back to previous menu
    default:
      printf("Error: Choice is not an option\n");
      break;
    }
    printf("1. Write a review\n2. List all active listings of user\n3. List all reviews of the user\n4. Back\nInput:\
 ");
    char f[20];
    fgets(f,20,stdin);
  }
  return;
}

void list_reviews(char* userEmail, sqlite3* db){
  char *zErrMsg = 0;
  const char* data = "Callback function called";

  char s[50];
  char search[100];
  
  printf("Enter email of user: ");
  scanf("%s",s);
  //list all reviews of user
  sprintf(search, "SELECT * FROM reviews a WHERE \"%s\"= a.reviewee", s);
  //printf("SQL: %s\n",search);
  int rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

  if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
    fprintf(stdout, "Operation done successfully\n");
  }
  
  
}

void list_listings(char* userEmail, sqlite3* db){
  char *zErrMsg = 0;
  const char* data = "Callback function called";

  char s[50];
  char search[100];
  printf("Enter email of user: ");
  scanf("%s",s);
  //SELECT * FROM sales s, products p WHERE s.pid = p.pid AND s.edate > time(\'now\')
  sprintf(search, "SELECT * FROM sales a WHERE \"%s\"= a.lister AND a.edate > datetime('now') ORDER BY a.edate", s);
  printf("SQL: %s\n",search);
  int rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

  if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
    fprintf(stdout, "Operation done successfully\n");
  }
  
}

void write_review(char* userEmail, sqlite3* db){
  //get all needed info and insert into table
  char *zErrMsg = 0;
  const char* data = "Callback function called";
  
  char s[50];
  char text[500];
  printf("Enter email of user: ");
  scanf("%s",s);
  printf("Enter review text: ");
  fgets(text,50,stdin);
  fgets(text,500,stdin);
  if(s[strlen(s)-1]=='\n')
    s[strlen(s)-1]='\0';
  printf("Enter rating: ");
  int rating=0;
  scanf("%d",&rating);
  char search[100];
  //insert review statement
  sprintf(search, "insert into reviews values ('%s', '%s', %d, '%s', date('now'))",userEmail,s,rating,text);
  //      printf("SQL: %s\n",search);
  int rc = sqlite3_exec(db, search,callback,(void*)data, &zErrMsg);

  if( rc != SQLITE_OK )
    {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    }else{
    fprintf(stdout, "Operation done successfully\n");
  }
}

//example
//char* SQL_pwFetch = strcat("SELECT u.password FROM users u WHERE u.email = email", email);
//sqlite3_exec(db, SQL_pwFetch,callback,(void*)data, &zErrMsg);


static int callback(void *data, int argc, char **argv, char **aColName)
{
  int i;
  fprintf(stderr, "%s: \n", (const char*)data);
  for(i = 0; i<argc; i++)
    {
      printf("%s = %s\n", aColName[i], argv[i] ? argv[i] : "NULL");
    }
  printf("\n");
  return 0;
}
