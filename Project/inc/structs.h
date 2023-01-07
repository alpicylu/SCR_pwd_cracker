#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <pthread.h>
#include <limits.h>

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
    int newly_cracked_idx; //mtx_flag_found. Used to pass the index of the cracked account to the cons thread.
} glData;

extern glData globalData;

extern int n_of_producers; //used by pthread_barrier_wait. We want producers to wait at the exit untill all producers finish, and then cancel the consuemr thread.

extern pthread_mutex_t mtx_pass;
extern pthread_mutex_t mtx_crack;
extern pthread_mutex_t mtx_passwds_cracked;
extern pthread_mutex_t mtx_flag_found;

extern pthread_cond_t cnd_pass_found; //condition that will signal the consummer that a new passowrd was found

extern pthread_barrier_t bar_producer_exit; //Producers will wait at their exits unitll every other porducer finishes their tasks.
//this is done to facilitate the consumer exiting once producers terminated.




#define NUMBER_APPEND_LIMIT 10000 //the maximal number that will be reached by the functions that append or prefix




#endif