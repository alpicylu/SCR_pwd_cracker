#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "parser.h"
#include "stringmod.h"
#include "breakers.h"
#include "structs.h"

/*to get a trully global structure define it in a header*/
/*REMEMBER TO FREE ALL MALLOCATED SPACE*/

glData globalData;

int main(int argc, char* argv[]){


    globalData.dbFilename = "/home/aleksander/scr2/10lista/DB.txt";
    globalData.dictFilename =  "/home/aleksander/scr2/10lista/dict.txt";

    initialise_dict();
    initialise_db();

    all_lowercase();
    all_uppercase();
    capitalised();

    view_result();

    cleanup();


}

/*Quick notes. Make sure to transport them to ToDo_C later.

    char** D = (char**)malloc(10*sizeof(char*));
    D[0] = "aaaa";
    printf("%s\n", D[0]);
    D[1] = "laptop";
    printf("%s\n", D[1]);
    return 0;
*/