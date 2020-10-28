#ifndef DRAGONFLY_STRUCT_H
#define DRAGONFLY_STRUCT_H


#include "std_common.h"
#include "ffc_group.h"

// 随机数
typedef struct{
    uint8_t* client_random;
    uint8_t* server_random;
    uint32_t random_len;
}Client_Server_Random;

// 密码
typedef struct{
    uint8_t* str;
    uint32_t len;
}Password;

// 用于生成提交元素的Priv_Mask
typedef struct{
    mpz_t priv;
    mpz_t mask;
}Priv_Mask;

// 提交元素Commit_Element
typedef struct{
    mpz_t scalar;
    mpz_t element;
}Commit_Element;


// 获取随机数状态的单例函数
gmp_randstate_t* get_randstate();

// 初始化PrviMask
void init_privmask(Priv_Mask* priv_mask);

// 使用随机值设置PrivMask
void set_privmask(Priv_Mask* priv_mask,const GMP_FFC_Group *ffc_group);

// 清理PrivMask
void clear_privmask(Priv_Mask* priv_mask);

// 初始化CommitElement
void init_commit_element(Commit_Element* commit_element);

// 清理CommitElement
void clear_commit_element(Commit_Element* commit_element);


#endif