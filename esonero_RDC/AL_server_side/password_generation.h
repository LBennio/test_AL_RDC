/*
 * password_generation.h
 *
 * Created 19 nov 2024
 * Author: Linda Germinario
 */

#ifndef PASSWORD_GENERATION_H_INCLUDED
#define PASSWORD_GENERATION_H_INCLUDED

void set_generation(char *type, int *len, char *str_in); // Function to parse input string and extract the password type and length
void password_generation(char *pass, char type, int length); // Function to parse input string and extract the password type and length
void generate_numeric(char *pass, int length); // Function to generate a numeric-only password
void generate_alpha(char *pass, int length); // Function to generate an alphabetic-only password (lowercase letters only)
void generate_mixed(char *pass, int length); // Function to generate a mixed alphanumeric password
void generate_secure(char *pass, int length); // Function to generate a secure password with uppercase, lowercase, digits, and symbols

#endif // PASSWORD_GENERATION_H_INCLUDED
