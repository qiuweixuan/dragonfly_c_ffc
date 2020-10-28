#include "../include/std_common.h"
#include "../include/dragonfly_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"
#include "../include/ffc_group.h"

/**
 * 要测试的函数：dragonfy_op.h / init_pwe
 * 功能：初始化密码元素PWE
 * 输入：
 * 1.客户端服务器随机数：const Client_Server_Random* randoms
 * 2.认证密码：const Password* password
 * 3.有限域运算元素：const GMP_FFC_Group* ffc_group
 * 4.迭代运算次数：const uint8_t k
 * 输出：
 * 1.密码元素：mpz_t password_element
 * 
 */
int main()
{
    // 1.客户端服务器随机数
    uint8_t client_random[] = {0x02,0x00,0x00,0x00,0x01,0x00}; 
    uint8_t server_random[] = {0x02,0x00,0x00,0x00,0x00,0x00}; 
    Client_Server_Random randoms = {
        .client_random = client_random,
        .server_random = server_random,
        .random_len = sizeof(client_random)
    };
    

    // 2.认证密码
    uint8_t password_str[] = "abcdefgh";
    Password pwd = {
        .str = password_str,
        .len = strlen(password_str)
    };

    // 3.有限域运算元素
    GMP_FFC_Group gmp_group15;
    gmp_ffc_group_factory_init(&gmp_group15,DH_GROUP15);


    //  4.迭代运算次数
    uint8_t k = 1;

    // 运算结果：密码元素
    mpz_t password_element;
    mpz_init(password_element);
    init_pwe(password_element,&randoms,&pwd,&gmp_group15,k);
    mpz_clear(password_element);

    gmp_ffc_group_clear(&gmp_group15);

    return 0;
}
