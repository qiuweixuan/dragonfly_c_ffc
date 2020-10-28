#include "../include/cstring.h"


void str2hex_str(uint8_t* hex_str,const uint8_t*str,const uint32_t str_len)
{
    uint8_t hexChars[] = "0123456789abcdef";
    uint32_t a, b;
    
    for (uint32_t i = 0; i < str_len; ++i)
    {
        uint8_t c = str[i];
        a = (c >> 4) & 0x0f;
        b = c & 0x0f;
        hex_str[2 * i] = hexChars[a];
        hex_str[2 * i + 1] = hexChars[b];
    }
}


int cstring_str2hex_str(CString* hex_str,const CString*str)
{   
    if(hex_str->len < 2 * str->len)
    {
        return -1;
    }

    uint8_t hexChars[] = "0123456789abcdef";
    uint32_t a, b;
    
    for (uint32_t i = 0; i < str->len; ++i)
    {
        uint8_t c = str->str[i];
        a = (c >> 4) & 0x0f;
        b = c & 0x0f;
        hex_str->str[2 * i] = hexChars[a];
        hex_str->str[2 * i + 1] = hexChars[b];
    }

    return 0;
}

void print_hex_str(const uint8_t*str,const uint32_t str_len)
{
    
    for (uint32_t i = 0; i < str_len; ++i)
    {
        printf("%02x",str[i]);
    }
    printf("\n");
}


void cstring_print_hex_str(const CString*str)
{
    
    for (uint32_t i = 0; i < str->len; ++i)
    {
        printf("%02x",str->str[i]);
    }
    printf("\n");
}


int cstring_hex_str2str(CString* rop,const CString* hex_op)
{
    // rop所需长度
    uint32_t rop_need_len = (hex_op->len / 2) + (hex_op->len % 2);
    if(rop_need_len < rop->len)
    {
        return -1;
    }

   
    
    uint32_t rop_index = 0;
    uint32_t hex_op_index = 0;

    if (hex_op->len % 2 == 1)
    {
        unsigned char v = 0;
        unsigned char c = hex_op->str[0];
        if (c >= '0' && c <= '9')
            v += (c - '0');
        else if (c >= 'a' && c <= 'f')
            v += (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            v += (c - 'A' + 10);

        rop->str[rop_index]  = v;
        rop_index++;
        hex_op_index++;
    }

    for (; hex_op_index < hex_op->len; hex_op_index += 2,rop_index++)
    {
        unsigned char v = 0;

        unsigned char c = hex_op->str[hex_op_index];
        if (c >= '0' && c <= '9')
            v += (c - '0') * 16;
        else if (c >= 'a' && c <= 'f')
            v += (c - 'a' + 10) * 16;
        else if (c >= 'A' && c <= 'F')
            v += (c - 'A' + 10) * 16;

        c = hex_op->str[hex_op_index + 1];
        if (c >= '0' && c <= '9')
            v += (c - '0');
        else if (c >= 'a' && c <= 'f')
            v += (c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            v += (c - 'A' + 10);

        rop->str[rop_index] = v;
    }
    return 0;
}


uint16_t transmute_u16_to_u8str(const uint16_t n, const U32Kind mode)
{

    uint16_t rop;
    uint8_t* u8str = (uint8_t*)&rop;

    if (mode == BE)
    {
        u8str[0] = (n >> 8) & 0xff;
        u8str[1] = n & 0xff;
    }
    else //默认为小端
    {
        u8str[1] = (n >> 8) & 0xff;
        u8str[0] = n & 0xff;
    }

    return rop;
}

uint8_t* memcpy_move_dstptr(uint8_t* dst,const uint8_t*src,const uint32_t len)
{
    memcpy(dst,src,len);
    dst += len;
    return dst;

}

void cstring_set_mpz(mpz_t gmp_op,const CString* str_op)
{
    // 分配足够的数组空间
    // uint8_t hex_str = malloc(str_len * 2 * sizeof(uint8_t) + 1);
    uint8_t hex_str[str_op->len * 2 + 1];
  
    // 字符串结尾加'\0'
    hex_str[str_op->len * 2] = '\0';

    // 初始化gmp_prime和gmp_order
    str2hex_str(hex_str,str_op->str,str_op->len);
    mpz_set_str(gmp_op,hex_str,16);
}

void mpz_print_hex(mpz_t op)
{
    int base = 16;
    unsigned int len = mpz_sizeinbase(op, base) + 2 + 1;
    char *rop = malloc(sizeof(char) * len);
    mpz_get_str(rop, base, op);
    rop[len-1]= '\0';
    printf("%s\n",rop);
    free(rop);
}