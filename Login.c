//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//login.c contains the login function used at the start of the program before the cmd line is used
//has two modes, either log in or sign up, which user decides
//Includes some adapted code from lecture slides

#include "Login.h"


int loginScreen() 
{
    //login screen is the parent function for both other 
    //loginScreen returns 1 if login was successful, 0 if failed

    int continueTop = 1;
    char userInput[2]; //only a single character is needed as list is numbered
    int signInResult = 0; //result of smaller sign-in functions

    printf("Welcome to CMPUT 291 Mini-Project 1. Login screen started.\n");

    while (continueTop) //top level loop
    {
        printf("---------------\nSelect an option:\n1: Sign in as existing user \n2: Create new user \n3: Quit\nInput: ");
        scanf("%s", userInput);

        if (userInput[0] == '1')
        {
            signInResult = signIn();
            if (signInResult == 1)
                return 1; //if sign in failed, return to top loop
        }
        else if (userInput[0] == '2')
        {
            signInResult = signUp();
            if (signInResult == 1)
                return 1;
        }
        else if (userInput[0] == '3')
        {
            printf("Quitting...\n");
            return 0;
        }
        else
        {
            printf("Invalid option. Please select a number from the list.\n");
        }
        
    }
    return 0;
}

int signIn()
{
    //Handles signing in with an existing username and password
    //if pw incorrect, return to top loop.
    //will include injection countering later
    int valid = 0;
    sqlite3 *db; char *zErrMsg = 0;
    int rc = 0;
    char realPW[20];

    sqlite3_open("test.db", &db);

    printf("Enter E-Mail: ");
    scanf("%s", userEmail);
    printf("Enter Password: ");
    scanf("%s", userPwd);
    if (strcmp(userEmail,"admin") == 0)
    {
        if (strcmp(userPwd, "admin") == 0)
        {
            printf("Login Successful, logging in as %s.\n", userEmail);
            return 1; //admin, admin ALWAYS WORKS
        }
    }

    //NEEDS COUNTERINJECTION HERE
    char SQL_pwFetch[100];
    sqlite3_stmt *stmt;

    sprintf(SQL_pwFetch, "SELECT u.pwd FROM users u WHERE lower(u.email) = lower(\"%s\");", userEmail);

    rc = sqlite3_prepare_v2(db, SQL_pwFetch,-1, &stmt, 0);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_step(stmt);
    

    if (rc == SQLITE_ROW) 
    {
        sprintf(realPW,"%s",sqlite3_column_text(stmt, 0));
        if (strcmp(realPW, userPwd) == 0)
            valid = 1;
    }
    

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (valid)
        printf("Login Successful. Signing in as %s", userEmail);
    else
        printf("Login Failed. E-Mail and Password do not match or do not exist.\n");
    
    return valid;
}

int signUp()
{
    //sign-up handles getting all info from user to create new row in user table.
    //!!all values other than pwd are converted to lower case !!
    
    //email and pwd already defined
    sqlite3 *db; int rc = 0;
    char *zErrMsg = 0;
    char name[16]; char city[16]; char gender[2];
    int inputting = 1;

    printf("Registering as a new user requires several fields of information.\n");
    printf("It requires a unique E-mail, along with name, password, city and gender.\n");
    printf("Type \"q\" at any time to quit.\n--------------------------\n");

    while (inputting)
    {
        printf("Enter E-mail (max chars 16): ");
        scanf("%s", userEmail);
        //must check to see if unique value
        if (strcmp(userEmail, "q") == 0)
            return 0;
        inputting = 0;
    }

    inputting = 1;
    while (inputting)
    {
        printf("Enter Password (max chars 4): ");
        scanf("%s", userPwd);
        //must check to see if unique value
        if (strcmp(userPwd, "q") == 0)
            return 0;
        inputting = 0;
    }

    inputting = 1;
    while (inputting)
    {
        printf("Enter name (max chars 16): ");
        scanf("%s", name);
        //must check to see if unique value
        if (strcmp(name, "q") == 0)
            return 0;
        inputting = 0;
    }

    inputting = 1;
    while (inputting)
    {
        printf("Enter city (max chars 15): ");
        scanf("%s", city);
        //must check to see if unique value
        if (strcmp(city, "q") == 0)
            return 0;
        inputting = 0;
    }

    inputting = 1;
    while (inputting)
    {
        printf("Enter gender (M or F): ");
        scanf("%s", gender);
        //must check to see if unique value
        if (strcmp(name, "q") == 0)
            return 0;
        inputting = 0;
    }

    printf("User Info: %s, %s, %s, %s, %s\n", userEmail,userPwd,name,city,gender);

    sqlite3_open("test.db", &db);

    char SQL_createUser[150];
    sprintf(SQL_createUser, "INSERT INTO users VALUES (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\");", userEmail, name, userPwd, city, gender);
    printf("%s",SQL_createUser);

    rc = sqlite3_exec(db, SQL_createUser,NULL,NULL, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        
    }
    else
    {
        printf("User Successfullly registered. Signing in as %s\n", userEmail);
        sqlite3_close(db);
        return 0;
    }
    
    sqlite3_close(db);
    return 0;
}