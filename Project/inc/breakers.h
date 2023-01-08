#ifndef BREAKERS_H
#define BREAKERS_H

#include "structs.h"
#include <time.h>

void hash_and_compare(const char* in);

void* all_lowercase(void* consumer_th); //this thread is also delegated to informing the consumer when all producers finished the work.

void* capitalised();

void* all_uppercase();

void* two_words_lowercase(); //this producer will take two words from the dictionary and attempt to brake the hash like that:
// word1 word1
// word1 word2 ...
// word2 word1

void* two_words_lowercase_numbers();

void* two_words_capitalised_uppercase();

void print_summary(float time);

void bytes2md5(const char *data, int len, char *md5buf);


#endif