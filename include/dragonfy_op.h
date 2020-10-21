#ifndef DRAGONFLY_OP_H
#define DRAGONFLY_OP_H

#include "std_common.h"
typedef struct{
    uint8_t* client_random;
    uint8_t* server_random;
    uint32_t random_len;
}Client_Server_Random;


typedef struct{
    uint8_t* str;
    uint32_t len;
}Password;


// init_pwe过程 / 计算哈希值子过程
void compute_hashed_password(uint8_t* digest,const Client_Server_Random* randoms,const Password* password ,const uint8_t counter);



#endif