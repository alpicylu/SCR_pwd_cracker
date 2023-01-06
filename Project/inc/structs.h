#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <pthread.h>

typedef struct {
    char hash[33]; //a hash will always be 32 chars in lenght, +1 for null terminator.
    char* email;
    char* passwd; //Thread-modifiable
    bool cracked; //Thread-modifiable
} user;

typedef struct {
    char* dictFilename;
    char* dbFilename;

    char** dict;
    user* users;

    unsigned int dict_len;
    unsigned int users_len;
    unsigned int passwd_found; //number of currently known passwords. Thread-modifiable
    bool flag_passwd_found; //variable for signaling the consoomer thread to print out the new cracked account. Thread-modifiable
} glData;

extern glData globalData;

extern pthread_mutex_t mtx_pass;
extern pthread_mutex_t mtx_crack;
extern pthread_mutex_t mtx_incr_found;
extern pthread_mutex_t mtx_flag_found;
extern pthread_cond_t cnd_pass_found;




#endif