#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "parser.h"
#include "stringmod.h"
#include "breakers.h"
#include "structs.h"

//defining global variables that were declared extern in structs.h
glData globalData;
pthread_mutex_t mtx_pass;
pthread_mutex_t mtx_crack;
pthread_mutex_t mtx_passwds_cracked;
pthread_mutex_t mtx_flag_found;
pthread_cond_t cnd_pass_found;
pthread_barrier_t bar_producer_exit;
int n_of_producers;

/*Make a function that will init global itself: set passwds_cracked to 0, set filenames, etc.*/

/*Initialises teh structure glData*/
void init_global(){
    globalData.dbFilename = "/home/aleksander/scr2/10lista/DB.txt";
    globalData.dictFilename =  "/home/aleksander/scr2/10lista/dict.txt";
    globalData.passwds_cracked = 0;
    globalData.flag_passwd_found = false;
    initialise_db();
    initialise_dict();
}

void* show_results(){

     //this mutex is literally just for cond_wait to compile properly.
    //initially wanted to just pass NULL, but the function does not allow it. Sad!
    //I actually need a mutex to reset the flag

    int idx;
    while (true){ 

        pthread_mutex_lock(&mtx_passwds_cracked);
        if (globalData.users_len == globalData.passwds_cracked) break;
        // if (globalData.passwds_cracked == 4) break;
        pthread_mutex_unlock(&mtx_passwds_cracked);

        pthread_mutex_lock(&mtx_flag_found);
        while (!globalData.flag_passwd_found){
            pthread_cond_wait(&cnd_pass_found, &mtx_flag_found);
        }
        globalData.flag_passwd_found = false;
        idx = globalData.newly_cracked_idx;
        printf("idx: %d ", idx); 
        pthread_mutex_unlock(&mtx_flag_found);

        pthread_mutex_lock(&mtx_pass);
        printf("%s - %s\n", globalData.users[idx].email, globalData.users[idx].passwd);
        pthread_mutex_unlock(&mtx_pass);

    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

    int n_threads = 4;
    n_of_producers = n_threads-1; //because we have one consumer

    pthread_t threads[n_threads];
    pthread_attr_t attr;

    init_global();

    pthread_mutex_init(&mtx_crack, NULL);
    pthread_mutex_init(&mtx_passwds_cracked, NULL);
    pthread_mutex_init(&mtx_pass, NULL);
    pthread_mutex_init(&mtx_flag_found, NULL);

    pthread_cond_init(&cnd_pass_found, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_barrier_init(&bar_producer_exit, NULL, n_of_producers);

    int oldtype;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
    
    pthread_create(&threads[1], &attr, all_lowercase, (void*)threads[0]);
    pthread_create(&threads[2], &attr, all_uppercase, (void*)threads[0]);
    pthread_create(&threads[3], &attr, capitalised, (void*)threads[0]);
    pthread_create(&threads[0], &attr, show_results, NULL); //consoomer
    
    // pthread_create(&threads[4], &attr, two_words_lowercase, NULL);
    

    for(int i=0; i<n_threads; ++i){
        pthread_join(threads[i], NULL);
    }

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mtx_crack);
    pthread_mutex_destroy(&mtx_passwds_cracked);
    pthread_mutex_destroy(&mtx_pass);
    pthread_cond_destroy(&cnd_pass_found);

    print_summary();

    cleanup();

    pthread_exit(NULL);

}



// void driver(){

//     globalData.dbFilename = "/home/aleksander/scr2/10lista/DB.txt";
//     globalData.dictFilename =  "/home/aleksander/scr2/10lista/dict.txt";
//     globalData.passwds_cracked = 0;

//     initialise_dict();
//     initialise_db();

//     all_lowercase(); //TH1
//     all_uppercase(); //TH2
//     capitalised(); //TH3

//     print_summary(); //TH0 (main)

//     cleanup();

// }

    /*implementing a separate structure for just cracked passwords and emails is redundant, since it would just be a copy of the
    content found in glData.
    But the only way for a thread to "return" a value is for it to edit an external structure which it has a pointer to.
    Maybe insted of returning a separate result (like a string containing email and password) implement a signal. Main upon
    recieving that signal will read the last "entry" from users - email and password of index -1.*/

    /*Or make a function that takes a single char* argument and prints it out. It will wait for new input until signaled.
    call this function in hash_and_compare*/

    /*well no, since the thread (function) will be running perpetually, and there will be only one consumer function, we cannot solve this by
    calling the consoomer multiple times. The results from HAC needs to be saved into a structure. Main thread will then in a loop attempt to read
    the contents of the structure.*/
    /*Make it so a flag is set everytime a new password is found. This flag will signal main to read the contents of the struct, but in such a way
    to print out only the last found password. Perhaps increment the total ammount of found passwords. The index of the last found password will then be equal
    to ammount-1.*/

    /*how exactly does pthread_cond_wait work? Because there are two conditions, one in the loop, second in the wait call.
    Theory: pthread_wait will not call if while evaluates to false, so thread skips past it.
    If, however, while is true, waiting will begin and it will end when the condition is signaled.
    so we got two signal layers: one that begins the waiting, and one that ends it.*/
    /*yea that makes sense, even the docs say "the blocked thread can be awakened by pthread_cond_signal()"*/

/*Quick notes. Make sure to transport them to ToDo_C later.

    char** D = (char**)malloc(10*sizeof(char*));
    D[0] = "aaaa";
    printf("%s\n", D[0]);
    D[1] = "laptop";
    printf("%s\n", D[1]);
    return 0;
*/