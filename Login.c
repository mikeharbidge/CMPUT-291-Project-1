//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//login.c contains the login function used at the start of the program before the cmd line is used
//has two modes, either log in or sign up, which user decides

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include "Login.h"

int main()
{
    //testing for login
    int in = 0; //if login succeeded
    in = loginScreen();
    if (in)
        printf("***LOGIN SUCCEEDED***\n");
    return 0;
}

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
        printf("Select an option:\n1: Sign in as existing user \n2: Create new user \n3: Quit\nInput: ");
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
    char username[20];
    char password[20];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    printf("%s, %s\n", username,password); //DEBUG
    if (strcmp(username,"admin") == 0)
        return 1;
    return 0;
}

int signUp()
{
    return 0;
}
