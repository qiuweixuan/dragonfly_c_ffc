#ifndef FFC_GROUP_H
#define FFC_GROUP_H
#include<gmp.h>

typedef enum 
{
    DH_GROUP5,DH_GROUP15
} FFC_GROUP_TYPE;

// 有限域基类
typedef struct
{
    const unsigned char* prime;
    const unsigned char* order;
    FFC_GROUP_TYPE group_type;
    unsigned int group_len;
}FFC_GROUP;

// 有限域GMP派生类
typedef struct
{   
    // 基类元素
    const FFC_GROUP* base;
    mpz_t gmp_prime;
    mpz_t gmp_order;
}GMP_FFC_GROUP;

// 有限域基类工厂
const FFC_GROUP* ffc_group_factory(const FFC_GROUP_TYPE group_type);

// 有限域GMP派生类工厂，对gmp_group进行初始化
int gmp_ffc_group_factory(GMP_FFC_GROUP* gmp_group, const FFC_GROUP_TYPE group_type);



#endif

