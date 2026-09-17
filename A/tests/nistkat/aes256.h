#ifndef B0_AES256_H
#define B0_AES256_H

void b0_aes256_encrypt_block(const unsigned char key[32],
                             const unsigned char in[16],
                             unsigned char out[16]);

#endif
