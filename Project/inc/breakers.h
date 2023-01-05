#ifndef BREAKERS_H
#define BREAKERS_H

#include "structs.h"

void hash_and_compare(const char* in);

void all_lowercase();

void capitalised();

void all_uppercase();

void view_result();

void bytes2md5(const char *data, int len, char *md5buf);


#endif