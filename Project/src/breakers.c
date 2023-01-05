
#include "breakers.h"
#include "stringmod.h"
#include <openssl/evp.h>
#include <string.h>

void hash_and_compare(const char* in, glData* data){
	char md5[33];
	bytes2md5(in, strlen(in), md5);
	printf("%s\n", in);

	//iterate over every user. If user has already beed cracked, skip to the next iteration.
	for(int idx=0; idx<data->users_len; ++idx){
		if(data->users[idx].cracked) continue;

		if( strcmp(data->users[idx].hash, md5) == 0 ){ //if the hashes are equal (see strcmp docs)
			data->users[idx].cracked = true;

			data->users[idx].passwd = (char*)malloc(strlen(in) * sizeof(char));
			strcpy(data->users[idx].passwd, in);
		}
	}
}

void all_lowercase(glData* data){
	int hardcoded = 20;
	char buf[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<data->dict_len; ++idx){ //iterating over the elements in the dict
			// hash_and_compare(data->dict[idx], data); 

			
			leading_num(data->dict[idx], buf, n);
			hash_and_compare(buf, data);

			if (n==-1) continue; //for n==-1 leading and trailing produce the same result.
			trailing_num(data->dict[idx], buf, n);
			hash_and_compare(buf, data);
		}
	}
}

void capitalised(glData* data){
	int hardcoded = 20;
	char buf[100];
	char word[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<data->dict_len; ++idx){ //iterating over the words in the dict
			capitalise(data->dict[idx], word);

			// hash_and_compare(word, data);

			leading_num(word, buf, n);
			hash_and_compare(buf, data);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf, data);
		}
	}
}

void all_uppercase(glData* data){
	int hardcoded = 20;
	char buf[100];
	char word[100];

	for(int n=-1; n<hardcoded; ++n){ //loop for prefixing and sufixing numbers to strings
		for(int idx=0; idx<data->dict_len; ++idx){ //iterating over the words in the dict
			uppercase(data->dict[idx], word);

			// hash_and_compare(word, data);

			leading_num(word, buf, n);
			hash_and_compare(buf, data);

			if (n==-1) continue;
			trailing_num(word, buf, n);
			hash_and_compare(buf, data);
		}
	}
}

void view_result(glData* data){
	for(int idx=0; idx<data->users_len; ++idx){
		if(data->users[idx].cracked){
			printf("%s - %s\n", data->users[idx].email, data->users[idx].passwd);
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