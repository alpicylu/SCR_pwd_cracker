#include "parser.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>


void remove_leading_whitespace(char* inp, char* out){
    int i=0;
    int k=0;

    while(inp[i] == ' ' || inp[i] == '\t' || inp[i] == '\n') ++i;
    for (int j=i; inp[j]!='\0'; ++j){
        out[k] = inp[j];
        ++k;
    }
    out[k] = '\0';
}

void initialise_db(){
    int usr_count = 0;
    char ch;
    int max_line = 256;
    char* token;
    int column = 0;
    int email_len = 1; //starting at one to account for \0
    char temp_str[100]; //used for remove_white_space. stores the current hash, and then email
    char line[max_line];
    FILE* db = fopen(globalData.dbFilename, "r");
    // printf("%s\n", globalData.dbFilename);

    if( db == NULL ){
        printf("DB: ERROR: At least one of the files have not been found.\n");
        exit(1);
    }

    //now, we need to initialise the user table. We need to know exactly how many users we forsee in our DB
    //we also need to know the length of each hash and emain addres for every user.
    //since hash len will always be 32+\0, this step is easy.

    //number of lines in DB.txt == number of users.
    while(!feof(db)){
        ch = fgetc(db);
        if(ch=='\n') ++usr_count;
    }
    globalData.users = (user*)malloc(usr_count*sizeof(user)); 
    globalData.users_len = usr_count; 
    rewind(db);

    //now for email address
    //1. read line
    //2. tokenise it
    //3. get elements at indexes 1 and 2
    //4. measure length of item at idx 2
    //5. allocate as much memory as was calculated in 4.
    //...

    for(int idx=0; idx<globalData.users_len; ++idx){ //for each user
        if (fgets(line, max_line, db) != NULL ){ //get a single line from db
            token = strtok(line, " ");
            while (token != NULL){ //iterate over columns for that line
            
                switch (column)
                {
                case 1: //hash
                    remove_leading_whitespace(token, temp_str);
                    strcpy(globalData.users[idx].hash, temp_str);
                    break;
                
                case 2: //email
                    for(int i=0; token[i] != '\0'; ++i) ++email_len; //calculate length of address (STRLEN EXISTS DUMMY)
                    globalData.users[idx].email = (char*)malloc(email_len*sizeof(char));
                    remove_leading_whitespace(token, temp_str);
                    strcpy(globalData.users[idx].email, temp_str);
                    break;

                default:
                    break;
                }

                token = strtok(NULL, " "); //get the next token. This syntax is rather counterintuitive, but thats how this function rolls.
                ++column;
            }
            globalData.users[idx].cracked = false; //initialising all entires as uncracked
            column = 0;
        }
    }
    fclose(db);
}



void initialise_dict(){
    int w_count = 0;
    int w_length = 1; //1 because we need to acconut for /0
    int max_len = 64;
    char ch;
    char buf[64];
    int dict_idx = 0;
    FILE* dict = fopen(globalData.dictFilename, "r");

    if( dict == NULL ){
        printf("ERROR: At least one of the files have not been found.\n");
        exit(1);
    }

    //w_count the number of words.
    while(!feof(dict)){
        ch = fgetc(dict);
        if(ch=='\n') ++w_count;
    }

    globalData.dict = (char**)malloc(w_count * sizeof(char*));
    globalData.dict_len = w_count;
    
    rewind(dict);

    //w_count the length of each word. Might as well read that word into memory.
    while(!feof(dict)){
        fscanf(dict, "%s", buf);
        for (int i=0; buf[i]!= '\0'; ++i) ++w_length; 

        globalData.dict[dict_idx] = (char*)malloc(w_length * sizeof(char));
        strcpy(globalData.dict[dict_idx], buf);
        ++dict_idx;
    }

    fclose(dict);
}

//malloc is used in: 
//  allocating the number of individual users - user
//  alloc space for each email address - user
//  alloc for number of words in the dict - dict
//  alloc for each word in dict according to its length - dict
//  in hash_and_compare to alloc for the found password - user
void cleanup(){
    for(int u=0; u<globalData.users_len; ++u){
        free(globalData.users[u].passwd);
        free(globalData.users[u].email);
    }
    // free(globalData.users); //we did not reserve space for every individual user, just space for n users.

    //for whatever reason, free(globalData.dict[0]) causes a double free error
    for(int d=0; d<globalData.dict_len; ++d){
        free(globalData.dict[d]);
    }

    free(globalData.dict);
}

