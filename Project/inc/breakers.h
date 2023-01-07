#ifndef BREAKERS_H
#define BREAKERS_H

#include "structs.h"
#include <pthread.h>

void hash_and_compare(const char* in);

void* all_lowercase(void* consumer_th);

void* capitalised(void* consumer_th);

void* all_uppercase(void* consumer_th);

void* two_words_lowercase(void* consumer_th); //this producer will take two words from the dictionary and attempt to brake the hash like that:
// word1 word1
// word1 word2 ...
// word2 word1

void* two_words_lowercase_numbers(void* consumer_th);

void print_summary();

void bytes2md5(const char *data, int len, char *md5buf);


#endif