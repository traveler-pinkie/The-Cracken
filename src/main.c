#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *hash = NULL;
    FILE *wordlist = NULL;


    if(argc != 3) {
        printf("Usage: %s <md5_hash> <wordlist_file>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <md5_hash> <wordlist_file>\n", argv[0]);
        return 0;
    }else{
        hash = argv[1];
        wordlist = fopen(argv[2], "r");
    }

    if(wordlist == NULL){
        printf("Error opening file\n");
        return 1;
    }

    if(strlen(hash) != 32){
        printf("Invalid hash length\n");
        return 1;
    }else{
        for(int i = 0;  i < 32; i++){
            if((hash[i] < '0' || hash[i] > '9' )&& (hash[i] < 'a' || hash[i] > 'f') && (hash[i] < 'A' || hash[i] > 'F')  ){
                printf("Invalid hash format\n");
                return 1;
            }
        }
    }

    fclose(wordlist);
    return 0;
}