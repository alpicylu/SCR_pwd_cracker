#ifndef STRINGMOD_H
#define STRINGMOD_H

//no need for lowercase funciton because source is already lowercase.
/*These funcitons generate strings that later will be md5-hashed.*/

void capitalise(char* in, char* out);

void uppercase(char* in, char* out);

void leading_num(char* in, char* out, int num);

void trailing_num(char* in, char* out, int num);

void two_words_space_between(char* in1, char* in2, char* out);

#endif