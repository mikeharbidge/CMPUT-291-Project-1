//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//main.c contains the main function of the program and calls all other functions
//Main also includes utility functions for opening and closing SQL databases
#include "main.h"

int main(int argc, char** argv){
  if (argv[1] != NULL)
  {
    printf("Opening database file: %s...\n", argv[1]);
    int rc = sqlite3_open(argv[1],&db);
    if(rc)
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return 0;
    }else
    {
      fprintf(stderr, "Opened database successfully\n");
    }
  }else
  {
    printf("No database argument given. Opening default database.\n");
    int rc = sqlite3_open("test.db",&db);
    if(rc)
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return 0;
    }else
    {
      fprintf(stderr, "Opened database successfully\n");
      
    }
  }
  
  int in = 0; //if login succeeded
  int choice;//input for choosing functionality
  in = loginScreen();
  if (in)
  {
	  do
    {
	    printf("1. List Products\n2. Search For Sales\n3. Post a Sale\n4. Search for Users\n5. Logout\nInput: ");
	    scanf("%d",&choice);
	    switch(choice){
	    case 1:
        list_products(db, userEmail);
	      break;
	    case 2:
        searchSales();
	      break;
	      //Search for Sales 
	    case 3:
		Post_Sale(userEmail,db);
	      break;
        //Post a sale
      case 4:
	      Search_Users(userEmail,db);
	      break;
	      //Search for users
	    case 5:
	      sqlite3_close(db);
	      return 0;
	      //logout and close DB
      default:
	      printf("Error: Choice is not an option\n");
        break;
      }
	  } while(1);
  }
  return 0;
}

