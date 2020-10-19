#include "../include/cstring.h"


void str2hex_str(unsigned char* hex_str,const unsigned char*str,const unsigned int str_len)
{
    unsigned char hexChars[] = "0123456789abcdef";
    unsigned int a, b;
    
    for (int i = 0; i < str_len; ++i)
    {
        unsigned char c = str[i];
        a = (c >> 4) & 0x0f;
        b = c & 0x0f;
        hex_str[2 * i] = hexChars[a];
        hex_str[2 * i + 1] = hexChars[b];
    }
    hex_str[2 * str_len] = '\0';
}