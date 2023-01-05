#ifndef PARSER_H
#define PARSER_H

#include "structs.h"

void remove_leading_whitespace(char*, char*);

void initialise_db(glData*);

void initialise_dict(glData*);

void cleanup(glData*);

#endif