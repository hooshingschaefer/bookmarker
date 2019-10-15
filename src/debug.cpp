#include "debug.h"
#include <sys/stat.h>
#include <cstdio>
#include <cstdint>
#include <time.h>
char file_name[16] = "bookmark_log1";
#define BASE_LOG_FILE 12
#define MAX_LOG_SIZE 100000000
int cur_log_end = 1;
char buf[50];
bool debug = false;

void debug_init(){
    debug = true;
    struct stat statter;
    //while the file exists increment the counter and copy into filename
    while(!stat(file_name, &statter) ){
        cur_log_end++;
        sprintf(file_name + BASE_LOG_FILE, "%d", cur_log_end);
    }
}

void l(const char* c...){
    //dont do anything unless debug mode is set
    if (debug){
        va_list args;
        struct stat statter;
        int int_arg;
        char* str_arg;
        time_t tb;
        struct tm* tp;
        va_start(args, c);
        time(&tb);
        tp = localtime(&tb);
        strftime(buf, 50, "%c", tp);
        if (!stat(file_name, &statter)){//if file exists
            int file_size = statter.st_blksize * statter.st_blocks;
            if (file_size > MAX_LOG_SIZE){
                cur_log_end++;
                sprintf(file_name + BASE_LOG_FILE, "%d", cur_log_end);
            }
        }
        
        FILE* fp = fopen(file_name, "a");
        fprintf(fp, "%s - ", buf);
        while(*c != '\0'){
            if(*c == '%'){
                c++;
                switch (*c){
                    case 'd':
                        int_arg = va_arg(args, int);
                        fprintf(fp, "%d", int_arg);
                        break;
                    case 's':
                        str_arg = va_arg(args, char*);
                        fprintf(fp, "%s", str_arg);
                        break;
                    //default:
                        
                        //break;
                }
            }else
                fprintf(fp,"%c", *c);
            c++;
        }
        fprintf(fp, "\n");
        va_end(args);
        fclose(fp);
    }
}

void d(const char* c...){
        //FILE* fp = fopen("bookmark.log", "a");
        va_list args;

        //
        int int_arg;
        char* str_arg;
        
        va_start(args, c);
        while(*c != '\0'){
            if(*c == '%'){
                c++;
                switch (*c){
                    case 'd':
                        int_arg = va_arg(args, int);
                        printf("%d", int_arg);
                        break;
                    case 's':
                        str_arg = va_arg(args, char*);
                        printf("%s", str_arg);
                        break;
                    //default:
                        
                        //break;
                }
            }else
                printf("%c", *c);
            c++;
        }
        printf("\n");
        va_end(args);
}

