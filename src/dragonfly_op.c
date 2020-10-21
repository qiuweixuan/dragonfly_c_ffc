#include "../include/dragonfy_op.h"
#include "../include/ssl.h"
#include "../include/cstring.h"

// 计算密码哈希值
void compute_hashed_password(uint8_t* digest,const Client_Server_Random* randoms,const Password* password ,const uint8_t counter)
{
    uint8_t* min_random;
    uint8_t* max_random;
    uint8_t* tmp_ptr = NULL;  

    // key = max_random | min_random 
    uint32_t key_len = randoms->random_len * 2;
    // 分配内存
    uint8_t* key = malloc(key_len  * sizeof(uint8_t));
    tmp_ptr = key;
    // 比较大小
    if(memcmp(randoms->client_random,randoms->server_random,randoms->random_len) < 0)
    {
        max_random = randoms->server_random;
        min_random = randoms->client_random;
    }
    else
    {
        max_random = randoms->client_random;
        min_random = randoms->server_random;
    }
    memcpy(tmp_ptr,max_random,randoms->random_len);
    tmp_ptr += randoms->random_len;
    memcpy(tmp_ptr,min_random,randoms->random_len);

    // message =  password |counter
    uint32_t message_len = password->len + sizeof(counter);
    // 分配内存
    uint8_t* message = malloc(message_len  * sizeof(uint8_t));
    tmp_ptr = message;
    // 拷贝数据
    memcpy(tmp_ptr,password->str,password->len);
    tmp_ptr += password->len;
    memcpy(tmp_ptr,&counter,sizeof(counter));

    // printf("key:\n");
    // print_hex_str(key,key_len);

    // printf("message:\n");
    // print_hex_str(message,message_len);

    
    // 计算哈希值
    hmac_sha256(digest,key,key_len,message,message_len);
    
    // printf("digest:\n");
    // print_hex_str(digest,32);

    // 释放内存
    free(message);
    free(key);
}



