#include "../include/std_common.h"
#include "../include/dragonfy_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"

int main()
{
    uint8_t password_str[] = "abcdefgh";
    CString pwd = {
        .str = password_str,
        .len = strlen(password_str)
    };
    uint8_t* hex_str = malloc((2 * pwd.len + 1) * sizeof(uint8_t));
    hex_str[2 * pwd.len] = '\0';

    CString hex_pwd = {
        .str = hex_str,
        .len = 2 * pwd.len
    };

    // str2hex_str
    cstring_str2hex_str(&hex_pwd,&pwd);
    printf("hex_str: %s\n",hex_pwd.str);
    
    // hex_str2str
    cstring_hex_str2str(&pwd,&hex_pwd);
    printf("str: %s\n",pwd.str);

    free(hex_str);

    // buf_len_bits:3072
    // bits_u8str:000c
    uint16_t buf_len_bits = 3072;
    uint16_t bits_u8str = transmute_u16_to_u8str(buf_len_bits,LE);
    printf("bits_u8str :");
    print_hex_str((uint8_t*)&bits_u8str,2);

    // buf_len_bits:1
    // bits_u8str:0100
    buf_len_bits = 1;
    bits_u8str = transmute_u16_to_u8str(buf_len_bits,LE);
    printf("bits_u8str :");
    print_hex_str((uint8_t*)&bits_u8str,2);

    return 0;
}
