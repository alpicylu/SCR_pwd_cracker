#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <pthread.h>

typedef struct {
    char hash[33]; //a hash will always be 32 chars in lenght, +1 for null terminator.
    char* email;
    char* passwd;
    bool cracked;
} user;

typedef struct {
    char* dictFilename;
    char* dbFilename;

    char** dict;
    user* users;

    unsigned int dict_len;
    unsigned int users_len;
} glData;

extern glData globalData;
extern pthread_mutex_t mutex;


#endif