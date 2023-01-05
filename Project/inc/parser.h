#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

void remove_leading_whitespace(char*, char*);

void initialise_db(); //globalise this first.

void initialise_dict();

void cleanup();

#endif