
#include "breakers.h"
#include "stringmod.h"
#include <openssl/evp.h>
#include <string.h>
#include <pthread.h>

//This appears to be the only function that writes to global. 
void hash_and_compare(const char* in){
	char md5[33];
	bytes2md5(in, strlen(in), md5);

	//iterate over every user. If user has already beed cracked, skip to the next iteration.
	for(int idx=0; idx<globalData.users_len; ++idx){
		if(globalData.users[idx].cracked) continue;

		if( strcmp(globalData.users[idx].hash, md5) == 0 ){ //if the hashes are equal (see strcmp docs)

			pthread_mutex_lock(&mtx_crack);
			globalData.users[idx].cracked = true; //Mutex this write
			pthread_mutex_unlock(&mtx_crack);

			pthread_mutex_lock(&mtx_pass);
			globalData.users[idx].passwd = (char*)malloc(strlen(in) * sizeof(char)); //Mutex this write
			strcpy(globalData.users[idx].passwd, in); //mutex this write
			pthread_mutex_unlock(&mtx_pass);

			pthread_mutex_lock(&mtx_passwds_cracked);
			++globalData.passwds_cracked; //Mutex this write
			pthread_mutex_unlock(&mtx_passwds_cracked);

			pthread_mutex_lock(&mtx_flag_found);
			globalData.flag_passwd_found = true; //MUTEX
			globalData.newly_cracked_idx = idx;
			pthread_mutex_unlock(&mtx_flag_found);

			pthread_cond_signal(&cnd_pass_found);
		}
	}
}

void* all_lowercase(){

	char buf[100];

	for(int n=-1; n<NUMBER_APPEND_LIMIT; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the elements in the dict
			
			leading_num(globalData.dict[idx], buf, n);
			hash_and_compare(buf);

			if (n==-1) continue; //for n==-1 leading and trailing produce the same result.
			trailing_num(globalData.dict[idx], buf, n);
			hash_and_compare(buf);
		}
	}
	// pthread_barrier_wait(&bar_producer_exit);
	pthread_exit(NULL);
}

void* capitalised(){

	char buf[100];
	char word[100];

	for(int n=-1; n<NUMBER_APPEND_LIMIT; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the words in the dict
			capitalise(globalData.dict[idx], word);

			leading_num(word, buf, n);
			hash_and_compare(buf);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf);
		}
	}
	// pthread_barrier_wait(&bar_producer_exit);
	pthread_exit(NULL);
}

void* all_uppercase(){
	char buf[100];
	char word[100];

	for(int n=-1; n<NUMBER_APPEND_LIMIT; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the words in the dict
			uppercase(globalData.dict[idx], word);

			leading_num(word, buf, n);
			hash_and_compare(buf);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf);
		}
	}
	// pthread_barrier_wait(&bar_producer_exit);
	pthread_exit(NULL);
}


void* two_words_lowercase(){

	char buf[100];

	for(int i=0; i<globalData.dict_len; ++i){
		for(int k=0; k<globalData.dict_len; ++k){
			two_words_space_between(globalData.dict[i], globalData.dict[k], buf);

			hash_and_compare(buf);
		}
	}
	// pthread_barrier_wait(&bar_producer_exit);
	pthread_exit(NULL);
}

void* two_words_lowercase_numbers(){

	char buf[100];
	char word1[100];
	char word2[100];

	for(int i=0; i<globalData.dict_len; ++i){
		for(int k=0; k<globalData.dict_len; ++k){
			for(int n=-1; n<NUMBER_APPEND_LIMIT; ++n){

				//3word cat
				leading_num(globalData.dict[i], word1, n); 
				two_words_space_between(word1, globalData.dict[k], buf);
				hash_and_compare(buf);

				//word3 cat
				trailing_num(globalData.dict[i], word1, n); 
				two_words_space_between(word1, globalData.dict[k], buf);
				hash_and_compare(buf);

				//word 3cat
				leading_num(globalData.dict[k], word2, n); 
				two_words_space_between(globalData.dict[i], word2, buf);
				hash_and_compare(buf);

				//word cat3
				trailing_num(globalData.dict[k], word2, n); 
				two_words_space_between(globalData.dict[i], word2, buf);
				hash_and_compare(buf);

				//3word 3cat
				leading_num(globalData.dict[i], word1, n);
				leading_num(globalData.dict[k], word2, n);
				two_words_space_between(word1, word2, buf);
				hash_and_compare(buf);

				//word3 3cat
				trailing_num(globalData.dict[i], word1, n);
				leading_num(globalData.dict[k], word2, n);
				two_words_space_between(word1, word2, buf);
				hash_and_compare(buf);

				//3word cat3
				leading_num(globalData.dict[i], word1, n);
				trailing_num(globalData.dict[k], word2, n);
				two_words_space_between(word1, word2, buf);
				hash_and_compare(buf);

				//word3 cat3
				trailing_num(globalData.dict[i], word1, n);
				trailing_num(globalData.dict[k], word2, n);
				two_words_space_between(word1, word2, buf);
				hash_and_compare(buf);

			}
		}
	}
	// pthread_barrier_wait(&bar_producer_exit);
	pthread_exit(NULL);
}

void* two_words_capitalised_uppercase(){

	char buf[100];
	char word1[100];
	char word2[100];

	for(int i=0; i<globalData.dict_len; ++i){
		for(int k=0; k<globalData.dict_len; ++k){
			//Word cat
			capitalise(globalData.dict[i], word1);
			two_words_space_between(word1, globalData.dict[k], buf);
			hash_and_compare(buf);

			//word Cat
			capitalise(globalData.dict[k], word2);
			two_words_space_between(globalData.dict[i], word2, buf);
			hash_and_compare(buf);

			//WORD cat
			uppercase(globalData.dict[i], word1);
			two_words_space_between(word1, globalData.dict[k], buf);
			hash_and_compare(buf);

			//word CAT
			uppercase(globalData.dict[k], word2);
			two_words_space_between(globalData.dict[i], word2, buf);
			hash_and_compare(buf);

			//Word Cat
			capitalise(globalData.dict[i], word1);
			capitalise(globalData.dict[k], word2);
			two_words_space_between(word1, word2, buf);
			hash_and_compare(buf);
			
			//WORD Cat
			uppercase(globalData.dict[i], word1);
			capitalise(globalData.dict[k], word2);
			two_words_space_between(word1, word2, buf);
			hash_and_compare(buf);

			//Word CAT
			capitalise(globalData.dict[i], word1);
			uppercase(globalData.dict[k], word2);
			two_words_space_between(word1, word2, buf);
			hash_and_compare(buf);

			//WORD CAT
			uppercase(globalData.dict[i], word1);
			uppercase(globalData.dict[k], word2);
			two_words_space_between(word1, word2, buf);
			hash_and_compare(buf);

		}
	}
	pthread_exit(NULL);
}


void print_summary(){
	printf("\n-------SUMMARY-------\n");
	for(int idx=0; idx<globalData.users_len; ++idx){
		if(globalData.users[idx].cracked){
			printf("%s - %s\n", globalData.users[idx].email, globalData.users[idx].passwd);
		}
		
	}
}

void bytes2md5(const char *data, int len, char *md5buf) {
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}