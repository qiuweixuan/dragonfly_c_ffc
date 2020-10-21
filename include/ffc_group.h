#ifndef FFC_GROUP_H
#define FFC_GROUP_H
#include<gmp.h>
#include "std_common.h"

typedef enum 
{
    DH_GROUP5,DH_GROUP15
} FFC_Group_Type;

// 有限域基类
typedef struct
{
    const uint8_t* prime;
    const uint8_t* order;
    FFC_Group_Type group_type;
    uint32_t group_len;
}FFC_Group;

// 有限域GMP派生类
typedef struct
{   
    // 基类元素
    const FFC_Group* base;
    mpz_t gmp_prime;
    mpz_t gmp_order;
}GMP_FFC_Group;

// 有限域基类工厂
const FFC_Group* ffc_group_factory(const FFC_Group_Type group_type);

// 有限域GMP派生类工厂，对gmp_group进行初始化
int gmp_ffc_group_factory(GMP_FFC_Group* gmp_group, const FFC_Group_Type group_type);



#endif

