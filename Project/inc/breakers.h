#ifndef BREAKERS_H
#define BREAKERS_H

#include "structs.h"

void hash_and_compare(const char* in, glData* data);

void all_lowercase(glData* data);

void capitalised(glData* data);

void all_uppercase(glData* data);

void view_result(glData* data);

void bytes2md5(const char *data, int len, char *md5buf);


#endif