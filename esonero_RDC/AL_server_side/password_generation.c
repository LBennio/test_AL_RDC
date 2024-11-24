/*
 * password_generation.c
 *
 * Created 19 nov 2024
 * Author: Linda Germinario
 */

#include "password_generation.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse input string and extract the password type and length
void set_generation(char *type, int *len, char *str_in) {
    // Tokenize the input string by spaces
    char *token = strtok(str_in, " ");

    // Iterate through tokens to retrieve password type and length
    for(int i = 1; (i <= 2) && token != NULL; i++) {
        switch(i) {
        case 1:
            strcpy(type, token); // First token is the password type
            break;
        case 2:
            *len = strtol(token, NULL, 10); // Second token is the password length
            break;
        }
        // Move to the next token
        token = strtok(NULL, " ");
    }
}

// Function to generate a password based on type and length
void password_generation(char *pass, char type, int length) {
    // Choose password generation method based on type
    switch(type) {
    case 'n':
        generate_numeric(pass, length); // Numeric password
        break;
    case 'a':
        generate_alpha(pass, length); // Alphabetic password
        break;
    case 'm':
        generate_mixed(pass, length); // Mixed alphanumeric password
        break;
    case 's':
        generate_secure(pass, length); // Secure password with special characters
        break;
    default:
        break;
    }
}

// Function to generate a numeric-only password
void generate_numeric(char *pass, int length) {
    srand(time(NULL)); // Seed the random number generator with current time

    for(int i = 0; i < length; i++) {
        pass[i] = rand() % 10 + '0'; // Generate random digit
    }

    pass[length] = '\0'; // Null-terminate the password string
}

// Function to generate an alphabetic-only password (lowercase letters only)
void generate_alpha(char *pass, int length) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int alphabet_size = strlen(alphabet) - 1;

    for(int i = 0; i < length; i++) {
        int random_index = rand() % alphabet_size; // Random index for alphabet
        pass[i] = alphabet[random_index]; // Set password character
    }

    pass[length] = '\0'; // Null-terminate the password string
}

// Function to generate a mixed alphanumeric password
void generate_mixed(char *pass, int length) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int alphabet_size = sizeof(alphabet) - 1;

    srand(time(NULL)); // Seed random number generator

    for (int i = 0; i < length; i++) {
        int c = rand();

        if(c % 2 == 0) { // Use a letter half the time
            int random_index = rand() % alphabet_size;
            pass[i] = alphabet[random_index];
            continue;
        }

        pass[i] = rand() % 10 + '0'; // Use a digit otherwise
    }

    pass[length] = '\0'; // Null-terminate the password string
}

// Function to generate a secure password with uppercase, lowercase, digits, and symbols
void generate_secure(char *pass, int length) {
    // Define character sets
    const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";
    const char symbols[] = "!@#$%^&*()-_=+[]{}|;:,.<>?";

    // Combined character set for general characters after initial four
    const char *all_characters = "abcdefghijklmnopqrstuvwxyz"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "0123456789"
                                 "!@#$%^&*()-_=+[]{}|;:,.<>?";

    int total_characters = sizeof(lowercase) - 1 +
                           sizeof(uppercase) - 1 +
                           sizeof(digits) - 1 +
                           sizeof(symbols) - 1;

    srand(time(NULL)); // Seed random number generator

    // Ensure inclusion of at least one character from each character set
    pass[0] = lowercase[rand() % (sizeof(lowercase) - 1)];
    pass[1] = uppercase[rand() % (sizeof(uppercase) - 1)];
    pass[2] = digits[rand() % (sizeof(digits) - 1)];
    pass[3] = symbols[rand() % (sizeof(symbols) - 1)];

    // Fill remaining characters randomly from all character types
    for (int i = 4; i < length; i++) {
        pass[i] = all_characters[rand() % (total_characters)];
    }

    pass[length] = '\0'; // Null-terminate the password string

    // Shuffle the password characters to randomize their order
    for (int i = 0; i < length; i++) {
        int j = rand() % length;
        char temp = pass[i];
        pass[i] = pass[j];
        pass[j] = temp;
    }
}
