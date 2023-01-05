#include <string.h>
#include <stdio.h>

/*possible race condition. since im modifying out multiple times from inside of this func
different threads might have different values of out, which is defined outside of this func.*/
void capitalise(char* in, char* out){
    strcpy(out, in);
    out[0] = out[0]-32;
}

void uppercase(char* in, char* out){
    strcpy(out, in);
    for(int i=0; out[i]!='\0'; ++i){
        out[i] = out[i]-32;
    }
}

void leading_num(char* in, char* out, int num){
    char numstring[10];
    char temp[100];
    if (num > -1){
        sprintf(numstring, "%d", num);
        strcpy(temp, in);
        strcpy(out, strcat(numstring, temp));
        return;
    }
    strcpy(out, in);
}

void trailing_num(char* in, char* out, int num){
    char numstring[10];
    char temp[100];
    if (num > -1){
        sprintf(numstring, "%d", num);
        strcpy(temp, in);
        strcpy(out, strcat(temp, numstring));
        return;
    }
    strcpy(out, in);
}