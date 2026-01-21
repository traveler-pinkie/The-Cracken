#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

int main(int argc, char *argv[]) {
    char *hash = NULL;
    FILE *wordlist = NULL;
    char buffer[256];
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;



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

    while(fgets(buffer, sizeof(buffer), wordlist) != NULL){
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        
        EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);
        EVP_DigestUpdate(mdctx, buffer, strlen(buffer));
        EVP_DigestFinal_ex(mdctx, digest, &digest_len);
        EVP_MD_CTX_free(mdctx);
        
        char computed_hash[33];
        for(int i = 0; i < 16; i++){
            sprintf(&computed_hash[i*2], "%02x", digest[i]);
        }
        computed_hash[32] = 0;

        if(strcasecmp(computed_hash, hash) == 0){
            printf("Password found: %s\n", buffer);
            fclose(wordlist);
            return 0;
        }
    }

    printf("Password not found\n");
    fclose(wordlist);
    return 0;
}