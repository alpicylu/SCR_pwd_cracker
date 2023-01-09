#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>
#include <pthread.h>
#include <limits.h>

#define NUMBER_APPEND_LIMIT 2500 //the maximal number that will be reached by the functions that append or prefix
#define MAX_FILENAME 100 //defines the maximum lengh of a filename/path to file
#define N_THREADS 7 //number of threads, including the consumer

//TODO: tidy up this file. Maybe put everything in one struct?

typedef struct {
    char hash[33]; //a hash will always be 32 chars in lenght, +1 for null terminator.
    char* email;
    char* passwd; //Thread-modifiable mtx_pass
    bool cracked; //Thread-modifiable mtx_crack
} user;

/*This structure contains data pertaining to user data, dictionaries and strictly password breaking.
Global variables regarding multithreading; mutexes, flags, conditions, barriers, are defined outsude
of this strucfutre.*/
typedef struct {
    char dictFilename[MAX_FILENAME];
    char dbFilename[MAX_FILENAME];

    char** dict;
    user* users;

    unsigned int dict_len;
    unsigned int users_len;
    unsigned int passwds_cracked; //number of currently known passwords. Thread-modifiable mtx_passwds_cracked
    bool flag_passwd_found; //variable for signaling the consoomer thread to print out the new cracked account. Thread-modifiable mtx_flag_found
    int newly_cracked_idx; //mtx_flag_found. Used to pass the index of the cracked account to the cons thread.
} glData;

extern glData globalData;

extern pthread_t threads[N_THREADS];

extern pthread_mutex_t mtx_pass;
extern pthread_mutex_t mtx_crack;
extern pthread_mutex_t mtx_passwds_cracked;
extern pthread_mutex_t mtx_flag_found;

extern pthread_barrier_t bar_producers_finished; 

extern pthread_cond_t cnd_pass_found; //condition that will signal the consummer that a new passowrd was found
//Used in conjunction with flag_passwd_found 

extern bool consumer_end_prematurely; //This flag will be used by the signal handler to prematurely terminate the
//program and display summary in the consumer thread





#endif