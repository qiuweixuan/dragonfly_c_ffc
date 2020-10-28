#ifndef FFC_GROUP_H
#define FFC_GROUP_H

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
    uint8_t safe_prime;
}FFC_Group;

// 有限域GMP派生类
typedef struct
{   
    // 基类元素
    const FFC_Group* base;
    mpz_t gmp_prime;
    mpz_t gmp_order;
}GMP_FFC_Group;

// 有限域基类工厂(单例模式)
const FFC_Group* ffc_group_factory(const FFC_Group_Type group_type);

// 有限域GMP派生类工厂，对gmp_group进行初始化
int gmp_ffc_group_factory_init(GMP_FFC_Group* gmp_group, const FFC_Group_Type group_type);

// 有限域GMP派生类清理
void gmp_ffc_group_clear(GMP_FFC_Group* gmp_group);

// 有限域标量运算
void ffc_scalar_op(mpz_t rop, const mpz_t op_exp, const mpz_t op_base,const GMP_FFC_Group *ffc_group);

// 有限域元素（向量）运算
void ffc_element_op(mpz_t rop, const mpz_t op1, const mpz_t op2,const GMP_FFC_Group *ffc_group);

// 有限域求逆运算
void ffc_inverse_op(mpz_t rop, const mpz_t op,const GMP_FFC_Group *ffc_group);

#endif

