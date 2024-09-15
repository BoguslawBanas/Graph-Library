#ifndef ERROR_INFO
#define ERROR_INFO

#include <cstdio>
#include <cstdint>
#include <cstdlib>

void printErrorMsg(const uint32_t code, const char *message){
    switch(code){
        case 1:{
            fprintf(stderr, "%s\n", message);
        } break;
        case 2:{
            fprintf(stderr, "%s\n", message);
            exit(1);
        } break;
    }
}

#endif

