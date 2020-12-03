#include <Python.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "lib/sha1.h"
#include <unistd.h>
#include "lib/encoding.h"

void HMAC_SHA1(uint8_t * key, uint8_t * m, uint8_t * hmac_result)
{
    //pad to 64 bytes
    int SHA1_Size = 64;
    uint8_t newKey[SHA1_Size];

    int i;
    for(i=0;i<SHA1_Size;i++){
        if (i < 10)
            newKey[i] = key[i];
        else
            newKey[i] = 0x00;
    }

    // array for holding inner and outer pad

    uint8_t K_opad[SHA1_Size];
    uint8_t K_ipad[SHA1_Size];

    // XOR key with i_pad and o_pad
    for(i=0;i<SHA1_Size;i++){
        K_opad[i] = 0x5c ^ newKey[i];
        K_ipad[i] = 0x36 ^ newKey[i];
    }

    // hash the innner value
    SHA1_INFO ctx_in;
    uint8_t hash_in[SHA1_DIGEST_LENGTH];
    sha1_init(&ctx_in);
    sha1_update(&ctx_in, K_ipad, SHA1_Size);
    sha1_update(&ctx_in, m,8);
    sha1_final(&ctx_in, hash_in);


    // hash the inner hashed value
    SHA1_INFO ctx_out;
    sha1_init(&ctx_out);
    sha1_update(&ctx_out, K_opad, SHA1_Size);
    sha1_update(&ctx_out, hash_in,SHA1_DIGEST_LENGTH);
    sha1_final(&ctx_out, hmac_result);

    return;
}

void genQR(){
    char filename [] = "google_auth.py";
    FILE* fp;
    Py_Initialize();
    fp = fopen(filename, "rx");
    PyRun_SimpleFile(fp, filename);
    Py_Finalize();
}

void genCode(){
    char secret [] = "theveryverysecrethex";
    uint8_t secret_byte[10];
    int i;
    for(i = 0; i < 10; i++){
        sscanf(&secret[i*2], "%02xd", &secret_byte[i]);
    }
    char result[100];
    base32_encode(secret_byte, 10, result, 100);
    //printf("%s\n",result);
    int T = time(NULL)/30;

    uint8_t counter[8];

    for(i = 7; i >= 0; i--){
        counter[i] = T;
        T = T >> 8;
    }

    uint8_t hmac_res[100];
    HMAC_SHA1(secret_byte, counter, hmac_res);
    int offset   =  hmac_res[19] & 0xf;
    int bin_code = (hmac_res[offset]  & 0x7f) << 24
        | (hmac_res[offset+1] & 0xff) << 16
        | (hmac_res[offset+2] & 0xff) <<  8
        | (hmac_res[offset+3] & 0xff) ;


    printf("%d\n", bin_code % 1000000);

}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("ERROR: no flags specified.");
        return 0;
    }
    char* flag = argv[1];
    if(strcmp(flag, "--generate-qr") == 0){
        genQR();
        return 0;
    }
    else if(strcmp(flag,"--get-otp") == 0){
        genCode();
    }
    else{
        printf("ERROR: Invalid flags");
        return 0;
    }
    return 0;
}


