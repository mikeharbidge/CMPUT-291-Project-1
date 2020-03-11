//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//allows user to post a sale described in spec
#include "PostSale.h"

void Post_Sale(char* userEmail, sqlite3* db){
  char *zErrMsg = 0;
  const char* data = "Callback function called";

  char z[50];
  char sid[50];char pid[50];char text[500];
  char search[100]; char cond[500];
  int days=0;
  int price=0;
  fgets(z,50,stdin);

  printf("Enter sale_id: ");
  fgets(sid,50,stdin);
  if(sid[strlen(sid)-1]=='\n')
    sid[strlen(sid)-1]='\0';
  printf("Enter review description: ");
  fgets(text,500,stdin);
  if(text[strlen(text)-1]=='\n')
    text[strlen(text)-1]='\0';
  printf("Enter condition: ");
  fgets(cond,500,stdin);
  if(cond[strlen(cond)-1]=='\n')
    cond[strlen(cond)-1]='\0';
  printf("How many days will the sale be up?");
  scanf("%d",&days);
  printf("What is the pid? ");
  scanf("%s",pid);
  printf("What is the price? ");
  scanf("%d",&price);
  //insert sale statement
  sprintf(search, "insert into sales values ('%s', '%s', '%s', datetime('now','+%d days'), '%s', '%s', %d)",sid,userEmail,pid,days,text,cond,price);
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
