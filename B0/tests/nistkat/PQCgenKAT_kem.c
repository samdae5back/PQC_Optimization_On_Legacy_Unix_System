#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rng.h"
#include "../../kem.h"

#define MAX_MARKER_LEN 50
#define KAT_SUCCESS 0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_DATA_ERROR -3
#define KAT_CRYPTO_FAILURE -4

static int FindMarker(FILE *infile, const char *marker);
static int ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
static void fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

int main(void)
{
    char fn_req[32];
    char fn_rsp[32];
    FILE *fp_req;
    FILE *fp_rsp;
    unsigned char seed[48];
    unsigned char entropy_input[48];
    unsigned char ct[CRYPTO_CIPHERTEXTBYTES];
    unsigned char ss[CRYPTO_BYTES];
    unsigned char ss1[CRYPTO_BYTES];
    unsigned char pk[CRYPTO_PUBLICKEYBYTES];
    unsigned char sk[CRYPTO_SECRETKEYBYTES];
    int count;
    int done;
    int ret_val;
    int i;

    sprintf(fn_req, "PQCkemKAT_%d.req", CRYPTO_SECRETKEYBYTES);
    fp_req = fopen(fn_req, "w");
    if(fp_req == NULL) {
        printf("Couldn't open <%s> for write\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }

    sprintf(fn_rsp, "PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    fp_rsp = fopen(fn_rsp, "w");
    if(fp_rsp == NULL) {
        fclose(fp_req);
        printf("Couldn't open <%s> for write\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }

    for(i = 0; i < 48; i++)
        entropy_input[i] = (unsigned char)i;

    randombytes_init(entropy_input, NULL, 256);

    for(i = 0; i < 100; i++) {
        fprintf(fp_req, "count = %d\n", i);
        randombytes(seed, 48);
        fprintBstr(fp_req, "seed = ", seed, 48);
        fprintf(fp_req, "pk =\n");
        fprintf(fp_req, "sk =\n");
        fprintf(fp_req, "ct =\n");
        fprintf(fp_req, "ss =\n\n");
    }

    fclose(fp_req);

    fp_req = fopen(fn_req, "r");
    if(fp_req == NULL) {
        fclose(fp_rsp);
        printf("Couldn't open <%s> for read\n", fn_req);
        return KAT_FILE_OPEN_ERROR;
    }

    fprintf(fp_rsp, "# %s\n\n", CRYPTO_ALGNAME);
    done = 0;

    do {
        if(FindMarker(fp_req, "count = "))
            fscanf(fp_req, "%d", &count);
        else {
            done = 1;
            break;
        }

        fprintf(fp_rsp, "count = %d\n", count);

        if(!ReadHex(fp_req, seed, 48, "seed = ")) {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_req);
            fclose(fp_req);
            fclose(fp_rsp);
            return KAT_DATA_ERROR;
        }

        fprintBstr(fp_rsp, "seed = ", seed, 48);
        randombytes_init(seed, NULL, 256);

        ret_val = crypto_kem_keypair(pk, sk);
        if(ret_val != 0) {
            printf("crypto_kem_keypair returned <%d>\n", ret_val);
            fclose(fp_req);
            fclose(fp_rsp);
            return KAT_CRYPTO_FAILURE;
        }

        fprintBstr(fp_rsp, "pk = ", pk, CRYPTO_PUBLICKEYBYTES);
        fprintBstr(fp_rsp, "sk = ", sk, CRYPTO_SECRETKEYBYTES);

        ret_val = crypto_kem_enc(ct, ss, pk);
        if(ret_val != 0) {
            printf("crypto_kem_enc returned <%d>\n", ret_val);
            fclose(fp_req);
            fclose(fp_rsp);
            return KAT_CRYPTO_FAILURE;
        }

        fprintBstr(fp_rsp, "ct = ", ct, CRYPTO_CIPHERTEXTBYTES);
        fprintBstr(fp_rsp, "ss = ", ss, CRYPTO_BYTES);
        fprintf(fp_rsp, "\n");

        ret_val = crypto_kem_dec(ss1, ct, sk);
        if(ret_val != 0) {
            printf("crypto_kem_dec returned <%d>\n", ret_val);
            fclose(fp_req);
            fclose(fp_rsp);
            return KAT_CRYPTO_FAILURE;
        }

        if(memcmp(ss, ss1, CRYPTO_BYTES) != 0) {
            printf("crypto_kem_dec returned bad 'ss' value\n");
            fclose(fp_req);
            fclose(fp_rsp);
            return KAT_CRYPTO_FAILURE;
        }
    } while(!done);

    fclose(fp_req);
    fclose(fp_rsp);
    return KAT_SUCCESS;
}

static int FindMarker(FILE *infile, const char *marker)
{
    char line[MAX_MARKER_LEN];
    int i;
    int len;
    int curr_line;

    len = (int)strlen(marker);
    if(len > MAX_MARKER_LEN - 1)
        len = MAX_MARKER_LEN - 1;

    for(i = 0; i < len; i++) {
        curr_line = fgetc(infile);
        line[i] = (char)curr_line;
        if(curr_line == EOF)
            return 0;
    }
    line[len] = '\0';

    while(1) {
        if(strncmp(line, marker, (size_t)len) == 0)
            return 1;

        for(i = 0; i < len - 1; i++)
            line[i] = line[i + 1];

        curr_line = fgetc(infile);
        line[len - 1] = (char)curr_line;
        if(curr_line == EOF)
            return 0;
        line[len] = '\0';
    }
}

static int ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
    int i;
    int ch;
    int started;
    unsigned char ich;

    if(Length == 0) {
        A[0] = 0x00;
        return 1;
    }

    memset(A, 0x00, (size_t)Length);
    started = 0;

    if(!FindMarker(infile, str))
        return 0;

    while((ch = fgetc(infile)) != EOF) {
        if(!isxdigit(ch)) {
            if(!started) {
                if(ch == '\n')
                    break;
                continue;
            }
            break;
        }

        started = 1;
        if(ch >= '0' && ch <= '9')
            ich = (unsigned char)(ch - '0');
        else if(ch >= 'A' && ch <= 'F')
            ich = (unsigned char)(ch - 'A' + 10);
        else if(ch >= 'a' && ch <= 'f')
            ich = (unsigned char)(ch - 'a' + 10);
        else
            ich = 0;

        for(i = 0; i < Length - 1; i++)
            A[i] = (unsigned char)((A[i] << 4) | (A[i + 1] >> 4));
        A[Length - 1] = (unsigned char)((A[Length - 1] << 4) | ich);
    }

    return 1;
}

static void fprintBstr(FILE *fp,
                       char *S,
                       unsigned char *A,
                       unsigned long long L)
{
    unsigned long long i;

    fprintf(fp, "%s", S);
    for(i = 0; i < L; i++)
        fprintf(fp, "%02X", A[i]);

    if(L == 0)
        fprintf(fp, "00");

    fprintf(fp, "\n");
}
