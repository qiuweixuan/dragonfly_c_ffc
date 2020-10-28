
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

//用于派生密钥的伪随机函数
void sha256_prf_bits(CString* result,const CString* key, CString* label,  CString* data, const uint16_t buf_len_bits)
{

    uint8_t* tmp_ptr;

    const uint16_t buf_len = (buf_len_bits + 7) / 8;
    
    uint16_t bits_u8str = transmute_u16_to_u8str(buf_len_bits, LE);

    // message = count_u8str | label | data | bits_u8str
    CString message;
    // 设置message长度，分配内存
    message.len = sizeof(uint16_t) + label->len + data->len + sizeof(uint16_t);
    message.str = malloc(message.len * sizeof(uint8_t));
    // 拷贝各个字段数据
    tmp_ptr = message.str;
    tmp_ptr += 2;
    tmp_ptr = memcpy_move_dstptr(tmp_ptr,label->str,label->len);
    tmp_ptr = memcpy_move_dstptr(tmp_ptr,data->str,data->len);
    tmp_ptr = memcpy_move_dstptr(tmp_ptr,(uint8_t*)&bits_u8str,sizeof(bits_u8str));
    
    // printf("message:");
    // cstring_print_hex_str(&message);

    uint32_t mac_len = HMAC_SHA256_DIGEST_SIZE;
    uint32_t pos = 0;
    uint16_t count = 1;
    uint8_t digest[HMAC_SHA256_DIGEST_SIZE];
    while (pos < buf_len)
    {
        uint32_t message_len = mac_len;
        if (buf_len - pos < mac_len)
        {
            message_len = buf_len - pos;
        }
        // set count_u8str
        uint16_t count_u8str = transmute_u16_to_u8str(count, LE);
        memcpy(message.str,(uint8_t*) &count_u8str, sizeof(count_u8str));
        
        // printf("handle_message:");
        // cstring_print_hex_str(&message);

        hmac_sha256(digest,key->str,key->len,message.str,message.len);

        // printf("key_digest:");
        // print_hex_str(digest,HMAC_SHA256_DIGEST_SIZE);

        memcpy(result->str + pos, digest, message_len);
        
        pos += message_len;
        count++;
    }

    // 释放message内存
    free(message.str);
}




