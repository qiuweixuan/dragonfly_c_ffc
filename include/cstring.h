#ifndef CSTRING_H
#define CSTRING_H

#include "std_common.h"


typedef struct 
{
    uint8_t* str;
    uint32_t len;
}CString;


typedef enum  
{
    BE,
    LE,
}U32Kind;


// 平凡字符串（"a"= 0x61 ）转十六进制ASCII码字符串("61")
void str2hex_str(uint8_t* hex_str,const uint8_t*str,const uint32_t str_len);
int cstring_str2hex_str(CString* hex_str,const CString*str);

// 打印平凡字符串十六进制数据
void print_hex_str(const uint8_t*str,const uint32_t str_len);
void cstring_print_hex_str(const CString*str);

// 十六进制ASCII码字符串("61")转平凡字符串（"a"= 0x61 ）
int cstring_hex_str2str(CString* rop,const CString* hex_op);

uint16_t transmute_u16_to_u8str(const uint16_t n, const U32Kind mode);

#endif