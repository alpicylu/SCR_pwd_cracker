#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "parser.h"
#include "stringmod.h"
#include "breakers.h"

/*to get a trully global structure define it in a header*/
/*REMEMBER TO FREE ALL MALLOCATED SPACE*/

int main(int argc, char* argv[]){

    glData global;
    global.dbFilename = "/home/aleksander/scr2/10lista/DB.txt";
    global.dictFilename =  "/home/aleksander/scr2/10lista/dict.txt";

    initialise_dict(&global);
    initialise_db(&global);

    all_lowercase(&global);
    all_uppercase(&global);
    capitalised(&global);

    view_result(&global);

    cleanup(&global);


}

/*Quick notes. Make sure to transport them to ToDo_C later.

    char** D = (char**)malloc(10*sizeof(char*));
    D[0] = "aaaa";
    printf("%s\n", D[0]);
    D[1] = "laptop";
    printf("%s\n", D[1]);
    return 0;
*/