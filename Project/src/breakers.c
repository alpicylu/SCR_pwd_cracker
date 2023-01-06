
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

			pthread_mutex_lock(&mtx_incr_found);
			++globalData.passwd_found; //Mutex this write
			pthread_mutex_unlock(&mtx_incr_found);

			pthread_mutex_lock(&mtx_flag_found);
			globalData.flag_passwd_found = true; //MUTEX
			pthread_mutex_unlock(&mtx_flag_found);

			pthread_cond_signal(&cnd_pass_found);
		}
	}
}

void* all_lowercase(){
	int hardcoded = 20;
	char buf[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the elements in the dict
			
			leading_num(globalData.dict[idx], buf, n);
			hash_and_compare(buf);

			if (n==-1) continue; //for n==-1 leading and trailing produce the same result.
			trailing_num(globalData.dict[idx], buf, n);
			hash_and_compare(buf);
		}
	}

	pthread_exit(NULL);
}

void* capitalised(){
	int hardcoded = 20;
	char buf[100];
	char word[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the words in the dict
			capitalise(globalData.dict[idx], word);

			leading_num(word, buf, n);
			hash_and_compare(buf);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf);
		}
	}

	pthread_exit(NULL);
}

void* all_uppercase(){
	int hardcoded = 20;
	char buf[100];
	char word[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<globalData.dict_len; ++idx){ //iterating over the words in the dict
			uppercase(globalData.dict[idx], word);

			// hash_and_compare(word, data);

			leading_num(word, buf, n);
			hash_and_compare(buf);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf);
		}
	}

	pthread_exit(NULL);
}

void view_result(){
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