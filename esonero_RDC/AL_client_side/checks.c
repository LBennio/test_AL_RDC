/*
 * checks.c
 *
 * Created 20 nov 2024
 * Author: Ennio Lo Buono
 */

#include "checks.h"

bool type_check(char *str) {
    if(strcmp(str, "n") == 0 ||    // check that the token is among the allowed options (n, m, a, s)
       strcmp(str, "m") == 0 ||
       strcmp(str, "a") == 0 ||
       strcmp(str, "s") == 0) {

        return true; // return true if it is allowed
    } else {
        return false; // return false if it is not allowed
    }
}


bool is_number(const char *str) {
    while (*str) {  // go trough every character part of the number
        if (!isdigit(*str)) {   // if ANY of the character is not a digit
            return false;       // return false
        }
        str++;
    }

    return true;    // if all characters are a digit, return true;
}

bool num_check(char *str) {

    if (!is_number(str)) {  // if the token is NOT a string of digits (so a number)
        return false;       // return false;
    }

    int num = atoi(str);    // get the number version of the token
    return (num >= MIN_INT && num <= MAX_INT);  // return true if the integer taken from the string is between the allowed options (6 - 32)
}

bool string_check(const char *str) {

    char temp_str[MAX_STR];
    strcpy(temp_str, str);  // copy the content of the string input in a temporary string

    if(strcmp(temp_str, "q") == 0) {    // "q" is a valid character, so
        return true;                    // if the string is "q", then return true, as it is valid
    }

    char *token = strtok(temp_str, " ");    // get the first part of the input, supposedly a number

    if(token == NULL) {     // if the string to tokenize does not have any valid character, return false
        return false;
    }

    unsigned int i = 1;     // control value

    while((i <= 2) && (token != NULL)) {    // check two tokens, for which they have to be two and present
        switch(i) {
            case 1:                         // control for the first token, supposedly a character

                if(!type_check(token)) {    // if the type is not among the allowed
                    printf("invalid %s\n", token);
                    return false;
                }

                token = strtok(NULL, "");   // get the second token, supposedly an integer

                if(token == NULL) {
                    return false;
                }
                break;
            case 2:
                if(!num_check(token)) {     // if the token is a number and is in limits
                    printf("invalid %s\n", token);
                    return false;
                }
                break;
        }

        i++;
    }

    return true;
}
