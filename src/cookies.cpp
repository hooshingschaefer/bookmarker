#include "cookies.h"
#include "protocol.h" //COOKIE_LEN
#include <stdlib.h> // random()

static char possible[62];


void Cookies::generate_cookie(char* c){
    for (int i =0; i < COOKIE_LEN; i++){
        c[i] = possible[random()%62];
    }
}

/*  populates the possible array with all alphanumeric characters
 *  so that they can be selected to generate a random string
 */
void Cookies::initialize(){
    for(int i=0 ; i < 10; i++){
        possible[i] = '0' + i;
    }
    for(int i=0 ; i < 26; i++){
        possible[i*2 + 10] = 'a' + i;
        possible[i*2 + 11] = 'A' + i;
    }
}
