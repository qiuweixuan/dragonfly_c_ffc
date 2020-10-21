
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include  "../include/ssl.h"

const uint32_t HMAC_SHA256_DIGEST_SIZE = 32;

//计算结果：32字节（256位）摘要digest
void hmac_sha256(uint8_t* digest,const uint8_t* key,const uint32_t key_len,const uint8_t* data, const uint32_t data_len)
{
    
    uint32_t hash_value_len = 0;
#if (OPENSSL_VERSION_NUMBER >= 0x10100001L)
    HMAC_CTX *ctx;

    ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key, key_len, EVP_sha256(), NULL);
    HMAC_Update(ctx, data, data_len);
    HMAC_Final(ctx, digest , &hash_value_len);
    HMAC_CTX_free(ctx);
#else
    HMAC_CTX ctx;

    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key, key_len, EVP_sha256(), NULL);
    HMAC_Update(&ctx, data, data_len);
    HMAC_Final(&ctx, digest , &hash_value_len);
    HMAC_CTX_cleanup(&ctx);
#endif

}