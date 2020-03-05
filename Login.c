//CMPUT 291 Mini-Project 1 Harbidge, Parhamglst, Wielgus
//login.c contains the login function used at the start of the program before the cmd line is used
//has two modes, either log in or sign up, which user decides

#include <stdio.h>
#include <sqlite3.h>

int loginScreen() {
//login screen is the parent function for both other 
//loginScreen returns 1 if login was successful, 0 if failed

int continueTop = 1;
char userInput = ''; //only a single character is needed as list is numbered

printf("Welcome to CMPUT 291 Mini-Project 1. Please select a sign-in option from the list below.");


while (continueTop){
    printf("\n1: Sign in as existing user \n2: Create new user \n3: Quit\nInput: ");
    scanf("%c", userInput);

    if (userInput == '1')
    {
        //sign-in with existing
    }
    else if (userInput == '2')
    {
        //Create new user
    }
    

    }



}