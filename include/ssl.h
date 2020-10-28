#ifndef SSL_H
#define SSL_H

#include "std_common.h"
#include "cstring.h"


const uint32_t HMAC_SHA256_DIGEST_SIZE;
void hmac_sha256(uint8_t* digest,const uint8_t* key,const uint32_t key_len,const uint8_t* data, const uint32_t data_len);
void sha256_prf_bits(CString* result,const CString* key, CString* label,  CString* data, const uint16_t buf_len_bits);

#endif