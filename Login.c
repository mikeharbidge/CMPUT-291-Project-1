//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//login.c contains the login function used at the start of the program before the cmd line is used
//has two modes, either log in or sign up, which user decides
//Includes some adapted code from lecture slides

#include "Login.h"

static int valid = 0;

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
    sqlite3 *db; char *zErrMsg = 0;
    int rc = 0;
    const char* data = "Callback function called";

    sqlite3_open("test.db", &db);

    int valid = 0;

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
    sprintf(SQL_pwFetch, "SELECT u.pwd FROM users u WHERE u.email = \"%s\"", userEmail);


    rc = sqlite3_exec(db, SQL_pwFetch,callback,(void *)data, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);

    valid += 1;

    if (valid)
        printf("Login Successful. Signing in as %s", userEmail);
    else
        printf("Login Failed. E-Mail and Password do not match or do not exist.\n");
    
    return valid;
}

int signUp()
{
    
    return 0;
}

static int callback(void *data, int argc, char **argv, char **aColName)
{
    //callback handles checking if the password given matches
    //if no password is returned from query, callback is ignored, automatically invalid

    printf("DEBUG: real password is: %s\n",argv[0]);
    //DOES NOT WORK FOR NOW
    return 0;
}