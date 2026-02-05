#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <pthread.h>
#include <stdatomic.h>
#include "hashes.h"

char *hash = NULL;
FILE *wordlist = NULL;
char buffer[256];
unsigned char digest[EVP_MAX_MD_SIZE];
unsigned int digest_len;
int num_threads;

atomic_int hashes_processed_counter = 0;
atomic_bool hashes_found_flag = 0;
atomic_int words_checked = 0;

struct threadData {
    char *hashString;
    FILE *filePointer;
    pthread_mutex_t mutex;
    int foundFlag;
    pthread_mutex_t *foundMutex;
    char *foundPassword;
};


int md5_verify(char* candidate, char* target){
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);
    EVP_DigestUpdate(mdctx, candidate, strlen(candidate));
    EVP_DigestFinal_ex(mdctx, digest, &digest_len);
    EVP_MD_CTX_free(mdctx);

    char computed_hash[33];
        for(int i = 0; i < 16; i++){
            sprintf(&computed_hash[i*2], "%02x", digest[i]);
        }
    computed_hash[32] = '\0';
    if(strcasecmp(computed_hash, target) == 0){
            return 1;
    }

    return 0;
}


int main(int argc, char *argv[]) {

    if(argc != 4) {
        printf("Usage: %s <number of threads> <md5_hash> <wordlist_file>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <number of threads> <md5_hash> <wordlist_file>\n", argv[0]);
        return 0;
    }else{
        hash = argv[2];
        num_threads = atoi(argv[1]);
        wordlist = fopen(argv[3], "r");
    }

    if(num_threads < 1 || num_threads > 128) {
    printf("Thread count must be between 1 and 128\n");
    return 1;
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
    while(fgets(buffer, sizeof(buffer), wordlist) != NULL){
        buffer[strcspn(buffer, "\n")] = '\0'; 
        if(md5_verify(buffer, hash)){
            printf("Password found: %s\n", buffer);
            fclose(wordlist);
            return 1;
        }
    }

    printf("Password not found\n");
    fclose(wordlist);
    return 0;
}
