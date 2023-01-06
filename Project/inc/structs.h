#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <pthread.h>

typedef struct {
    char hash[33]; //a hash will always be 32 chars in lenght, +1 for null terminator.
    char* email;
    char* passwd; //Thread-modifiable mtx_pass
    bool cracked; //Thread-modifiable mtx_crack
} user;

typedef struct {
    char* dictFilename;
    char* dbFilename;

    char** dict;
    user* users;

    unsigned int dict_len;
    unsigned int users_len;
    unsigned int passwds_cracked; //number of currently known passwords. Thread-modifiable mtx_passwds_cracked
    bool flag_passwd_found; //variable for signaling the consoomer thread to print out the new cracked account. Thread-modifiable mtx_flag_found
    int newly_cracked_idx; //mtx_flag_found
} glData;

extern glData globalData;

extern pthread_mutex_t mtx_pass;
extern pthread_mutex_t mtx_crack;
extern pthread_mutex_t mtx_passwds_cracked;
extern pthread_mutex_t mtx_flag_found;
extern pthread_cond_t cnd_pass_found;




#endif