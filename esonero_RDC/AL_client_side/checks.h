/*
 * check.h
 *
 * Created 20 nov 2024
 * author* Ennio Lo Buono
 */

#ifndef CHECKS_H_INCLUDED
#define CHECKS_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_INT 6
#define MAX_INT 32

#define MAX_STR 30
#define MIN_STR 3

bool string_check(const char *); // function that checks if a string is correct by program criteria
bool type_check(char *str); // function that checks if str is among the allowed strings

#endif // CHECKS_H_INCLUDED
