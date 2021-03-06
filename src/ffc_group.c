#include "../include/ffc_group.h"
#include "../include/cstring.h"

static const uint8_t dh_group5_prime[192] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
    0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22,
    0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B,
    0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
    0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B,
    0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5,
    0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
    0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
    0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05,
    0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A,
    0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
    0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96,
    0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB,
    0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
    0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04,
    0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x23, 0x73, 0x27,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static const uint8_t dh_group5_order[192] = {
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE4, 0x87, 0xED, 0x51, 0x10, 0xB4, 0x61, 0x1A,
    0x62, 0x63, 0x31, 0x45, 0xC0, 0x6E, 0x0E, 0x68,
    0x94, 0x81, 0x27, 0x04, 0x45, 0x33, 0xE6, 0x3A,
    0x01, 0x05, 0xDF, 0x53, 0x1D, 0x89, 0xCD, 0x91,
    0x28, 0xA5, 0x04, 0x3C, 0xC7, 0x1A, 0x02, 0x6E,
    0xF7, 0xCA, 0x8C, 0xD9, 0xE6, 0x9D, 0x21, 0x8D,
    0x98, 0x15, 0x85, 0x36, 0xF9, 0x2F, 0x8A, 0x1B,
    0xA7, 0xF0, 0x9A, 0xB6, 0xB6, 0xA8, 0xE1, 0x22,
    0xF2, 0x42, 0xDA, 0xBB, 0x31, 0x2F, 0x3F, 0x63,
    0x7A, 0x26, 0x21, 0x74, 0xD3, 0x1B, 0xF6, 0xB5,
    0x85, 0xFF, 0xAE, 0x5B, 0x7A, 0x03, 0x5B, 0xF6,
    0xF7, 0x1C, 0x35, 0xFD, 0xAD, 0x44, 0xCF, 0xD2,
    0xD7, 0x4F, 0x92, 0x08, 0xBE, 0x25, 0x8F, 0xF3,
    0x24, 0x94, 0x33, 0x28, 0xF6, 0x72, 0x2D, 0x9E,
    0xE1, 0x00, 0x3E, 0x5C, 0x50, 0xB1, 0xDF, 0x82,
    0xCC, 0x6D, 0x24, 0x1B, 0x0E, 0x2A, 0xE9, 0xCD,
    0x34, 0x8B, 0x1F, 0xD4, 0x7E, 0x92, 0x67, 0xAF,
    0xC1, 0xB2, 0xAE, 0x91, 0xEE, 0x51, 0xD6, 0xCB,
    0x0E, 0x31, 0x79, 0xAB, 0x10, 0x42, 0xA9, 0x5D,
    0xCF, 0x6A, 0x94, 0x83, 0xB8, 0x4B, 0x4B, 0x36,
    0xB3, 0x86, 0x1A, 0xA7, 0x25, 0x5E, 0x4C, 0x02,
    0x78, 0xBA, 0x36, 0x04, 0x65, 0x11, 0xB9, 0x93,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static const uint8_t dh_group15_prime[384] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC9, 0x0F, 0xDA, 0xA2, 0x21, 0x68, 0xC2, 0x34,
    0xC4, 0xC6, 0x62, 0x8B, 0x80, 0xDC, 0x1C, 0xD1,
    0x29, 0x02, 0x4E, 0x08, 0x8A, 0x67, 0xCC, 0x74,
    0x02, 0x0B, 0xBE, 0xA6, 0x3B, 0x13, 0x9B, 0x22,
    0x51, 0x4A, 0x08, 0x79, 0x8E, 0x34, 0x04, 0xDD,
    0xEF, 0x95, 0x19, 0xB3, 0xCD, 0x3A, 0x43, 0x1B,
    0x30, 0x2B, 0x0A, 0x6D, 0xF2, 0x5F, 0x14, 0x37,
    0x4F, 0xE1, 0x35, 0x6D, 0x6D, 0x51, 0xC2, 0x45,
    0xE4, 0x85, 0xB5, 0x76, 0x62, 0x5E, 0x7E, 0xC6,
    0xF4, 0x4C, 0x42, 0xE9, 0xA6, 0x37, 0xED, 0x6B,
    0x0B, 0xFF, 0x5C, 0xB6, 0xF4, 0x06, 0xB7, 0xED,
    0xEE, 0x38, 0x6B, 0xFB, 0x5A, 0x89, 0x9F, 0xA5,
    0xAE, 0x9F, 0x24, 0x11, 0x7C, 0x4B, 0x1F, 0xE6,
    0x49, 0x28, 0x66, 0x51, 0xEC, 0xE4, 0x5B, 0x3D,
    0xC2, 0x00, 0x7C, 0xB8, 0xA1, 0x63, 0xBF, 0x05,
    0x98, 0xDA, 0x48, 0x36, 0x1C, 0x55, 0xD3, 0x9A,
    0x69, 0x16, 0x3F, 0xA8, 0xFD, 0x24, 0xCF, 0x5F,
    0x83, 0x65, 0x5D, 0x23, 0xDC, 0xA3, 0xAD, 0x96,
    0x1C, 0x62, 0xF3, 0x56, 0x20, 0x85, 0x52, 0xBB,
    0x9E, 0xD5, 0x29, 0x07, 0x70, 0x96, 0x96, 0x6D,
    0x67, 0x0C, 0x35, 0x4E, 0x4A, 0xBC, 0x98, 0x04,
    0xF1, 0x74, 0x6C, 0x08, 0xCA, 0x18, 0x21, 0x7C,
    0x32, 0x90, 0x5E, 0x46, 0x2E, 0x36, 0xCE, 0x3B,
    0xE3, 0x9E, 0x77, 0x2C, 0x18, 0x0E, 0x86, 0x03,
    0x9B, 0x27, 0x83, 0xA2, 0xEC, 0x07, 0xA2, 0x8F,
    0xB5, 0xC5, 0x5D, 0xF0, 0x6F, 0x4C, 0x52, 0xC9,
    0xDE, 0x2B, 0xCB, 0xF6, 0x95, 0x58, 0x17, 0x18,
    0x39, 0x95, 0x49, 0x7C, 0xEA, 0x95, 0x6A, 0xE5,
    0x15, 0xD2, 0x26, 0x18, 0x98, 0xFA, 0x05, 0x10,
    0x15, 0x72, 0x8E, 0x5A, 0x8A, 0xAA, 0xC4, 0x2D,
    0xAD, 0x33, 0x17, 0x0D, 0x04, 0x50, 0x7A, 0x33,
    0xA8, 0x55, 0x21, 0xAB, 0xDF, 0x1C, 0xBA, 0x64,
    0xEC, 0xFB, 0x85, 0x04, 0x58, 0xDB, 0xEF, 0x0A,
    0x8A, 0xEA, 0x71, 0x57, 0x5D, 0x06, 0x0C, 0x7D,
    0xB3, 0x97, 0x0F, 0x85, 0xA6, 0xE1, 0xE4, 0xC7,
    0xAB, 0xF5, 0xAE, 0x8C, 0xDB, 0x09, 0x33, 0xD7,
    0x1E, 0x8C, 0x94, 0xE0, 0x4A, 0x25, 0x61, 0x9D,
    0xCE, 0xE3, 0xD2, 0x26, 0x1A, 0xD2, 0xEE, 0x6B,
    0xF1, 0x2F, 0xFA, 0x06, 0xD9, 0x8A, 0x08, 0x64,
    0xD8, 0x76, 0x02, 0x73, 0x3E, 0xC8, 0x6A, 0x64,
    0x52, 0x1F, 0x2B, 0x18, 0x17, 0x7B, 0x20, 0x0C,
    0xBB, 0xE1, 0x17, 0x57, 0x7A, 0x61, 0x5D, 0x6C,
    0x77, 0x09, 0x88, 0xC0, 0xBA, 0xD9, 0x46, 0xE2,
    0x08, 0xE2, 0x4F, 0xA0, 0x74, 0xE5, 0xAB, 0x31,
    0x43, 0xDB, 0x5B, 0xFC, 0xE0, 0xFD, 0x10, 0x8E,
    0x4B, 0x82, 0xD1, 0x20, 0xA9, 0x3A, 0xD2, 0xCA,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t dh_group15_order[384] = {
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE4, 0x87, 0xED, 0x51, 0x10, 0xB4, 0x61, 0x1A,
    0x62, 0x63, 0x31, 0x45, 0xC0, 0x6E, 0x0E, 0x68,
    0x94, 0x81, 0x27, 0x04, 0x45, 0x33, 0xE6, 0x3A,
    0x01, 0x05, 0xDF, 0x53, 0x1D, 0x89, 0xCD, 0x91,
    0x28, 0xA5, 0x04, 0x3C, 0xC7, 0x1A, 0x02, 0x6E,
    0xF7, 0xCA, 0x8C, 0xD9, 0xE6, 0x9D, 0x21, 0x8D,
    0x98, 0x15, 0x85, 0x36, 0xF9, 0x2F, 0x8A, 0x1B,
    0xA7, 0xF0, 0x9A, 0xB6, 0xB6, 0xA8, 0xE1, 0x22,
    0xF2, 0x42, 0xDA, 0xBB, 0x31, 0x2F, 0x3F, 0x63,
    0x7A, 0x26, 0x21, 0x74, 0xD3, 0x1B, 0xF6, 0xB5,
    0x85, 0xFF, 0xAE, 0x5B, 0x7A, 0x03, 0x5B, 0xF6,
    0xF7, 0x1C, 0x35, 0xFD, 0xAD, 0x44, 0xCF, 0xD2,
    0xD7, 0x4F, 0x92, 0x08, 0xBE, 0x25, 0x8F, 0xF3,
    0x24, 0x94, 0x33, 0x28, 0xF6, 0x72, 0x2D, 0x9E,
    0xE1, 0x00, 0x3E, 0x5C, 0x50, 0xB1, 0xDF, 0x82,
    0xCC, 0x6D, 0x24, 0x1B, 0x0E, 0x2A, 0xE9, 0xCD,
    0x34, 0x8B, 0x1F, 0xD4, 0x7E, 0x92, 0x67, 0xAF,
    0xC1, 0xB2, 0xAE, 0x91, 0xEE, 0x51, 0xD6, 0xCB,
    0x0E, 0x31, 0x79, 0xAB, 0x10, 0x42, 0xA9, 0x5D,
    0xCF, 0x6A, 0x94, 0x83, 0xB8, 0x4B, 0x4B, 0x36,
    0xB3, 0x86, 0x1A, 0xA7, 0x25, 0x5E, 0x4C, 0x02,
    0x78, 0xBA, 0x36, 0x04, 0x65, 0x0C, 0x10, 0xBE,
    0x19, 0x48, 0x2F, 0x23, 0x17, 0x1B, 0x67, 0x1D,
    0xF1, 0xCF, 0x3B, 0x96, 0x0C, 0x07, 0x43, 0x01,
    0xCD, 0x93, 0xC1, 0xD1, 0x76, 0x03, 0xD1, 0x47,
    0xDA, 0xE2, 0xAE, 0xF8, 0x37, 0xA6, 0x29, 0x64,
    0xEF, 0x15, 0xE5, 0xFB, 0x4A, 0xAC, 0x0B, 0x8C,
    0x1C, 0xCA, 0xA4, 0xBE, 0x75, 0x4A, 0xB5, 0x72,
    0x8A, 0xE9, 0x13, 0x0C, 0x4C, 0x7D, 0x02, 0x88,
    0x0A, 0xB9, 0x47, 0x2D, 0x45, 0x55, 0x62, 0x16,
    0xD6, 0x99, 0x8B, 0x86, 0x82, 0x28, 0x3D, 0x19,
    0xD4, 0x2A, 0x90, 0xD5, 0xEF, 0x8E, 0x5D, 0x32,
    0x76, 0x7D, 0xC2, 0x82, 0x2C, 0x6D, 0xF7, 0x85,
    0x45, 0x75, 0x38, 0xAB, 0xAE, 0x83, 0x06, 0x3E,
    0xD9, 0xCB, 0x87, 0xC2, 0xD3, 0x70, 0xF2, 0x63,
    0xD5, 0xFA, 0xD7, 0x46, 0x6D, 0x84, 0x99, 0xEB,
    0x8F, 0x46, 0x4A, 0x70, 0x25, 0x12, 0xB0, 0xCE,
    0xE7, 0x71, 0xE9, 0x13, 0x0D, 0x69, 0x77, 0x35,
    0xF8, 0x97, 0xFD, 0x03, 0x6C, 0xC5, 0x04, 0x32,
    0x6C, 0x3B, 0x01, 0x39, 0x9F, 0x64, 0x35, 0x32,
    0x29, 0x0F, 0x95, 0x8C, 0x0B, 0xBD, 0x90, 0x06,
    0x5D, 0xF0, 0x8B, 0xAB, 0xBD, 0x30, 0xAE, 0xB6,
    0x3B, 0x84, 0xC4, 0x60, 0x5D, 0x6C, 0xA3, 0x71,
    0x04, 0x71, 0x27, 0xD0, 0x3A, 0x72, 0xD5, 0x98,
    0xA1, 0xED, 0xAD, 0xFE, 0x70, 0x7E, 0x88, 0x47,
    0x25, 0xC1, 0x68, 0x90, 0x54, 0x9D, 0x69, 0x65,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static const FFC_Group GROUP5 = {
    .prime = dh_group5_prime,
    .order = dh_group5_order,
    .group_type = DH_GROUP5,
    .group_len = sizeof(dh_group5_prime),
    .safe_prime = 1
};

static const FFC_Group GROUP15 = {
    .prime = dh_group15_prime,
    .order = dh_group15_order,
    .group_type = DH_GROUP15,
    .group_len = sizeof(dh_group15_prime),
    .safe_prime = 1
};

// 构建有限域群的工厂方法
const FFC_Group *ffc_group_factory(const FFC_Group_Type group_type)
{
    const FFC_Group* res;
    switch (group_type)
    {
    case DH_GROUP5:
        res = &GROUP5;
        break;
    case DH_GROUP15:
        res = &GROUP15;
        break;
    default:
        res = NULL;
        break;
    }
}
// 初始化GMP有限域群的工厂方法
int gmp_ffc_group_factory_init(GMP_FFC_Group* gmp_group, const FFC_Group_Type group_type)
{
    if(gmp_group == NULL){
        return -1;
    }

    switch (group_type)
    {
    case DH_GROUP5:
        gmp_group->base= &GROUP5;
        break;
    case DH_GROUP15:
        gmp_group->base = &GROUP15;
        break;
    default:
        return -1;
    }
    
    uint32_t str_len =  gmp_group->base->group_len;
    // 分配足够的数组空间
    uint8_t* hex_str = (uint8_t*)malloc(str_len * 2 * sizeof(uint8_t) + 1);
    // 字符串结尾加'\0'
    hex_str[str_len * 2] = '\0';

    // 初始化gmp_prime和gmp_order
    str2hex_str(hex_str,gmp_group->base->prime,str_len);
    mpz_init_set_str(gmp_group->gmp_prime,hex_str,16);
    str2hex_str(hex_str,gmp_group->base->order,str_len);
    mpz_init_set_str(gmp_group->gmp_order,hex_str,16);
    // 释放数组空间
    free(hex_str);
    return 0;
}



// 清理GMP有限域群的方法
void gmp_ffc_group_clear(GMP_FFC_Group* gmp_group)
{
    if(gmp_group == NULL){
        return;
    }
    mpz_clear(gmp_group->gmp_prime);
    mpz_clear(gmp_group->gmp_order);
}

// 有限域标量运算
void ffc_scalar_op(mpz_t rop, const mpz_t op_exp, const mpz_t op_base,const GMP_FFC_Group *ffc_group)
{
    mpz_powm(rop, op_base, op_exp, ffc_group->gmp_prime);
}
// 有限域元素（向量）运算
void ffc_element_op(mpz_t rop, const mpz_t op1, const mpz_t op2,const GMP_FFC_Group *ffc_group)
{
    mpz_mul(rop, op1, op2);
    mpz_mod(rop, rop, ffc_group->gmp_prime);
}
// 有限域求逆运算
void ffc_inverse_op(mpz_t rop, const mpz_t op,const GMP_FFC_Group *ffc_group)
{
    mpz_invert(rop, op, ffc_group->gmp_prime);
}