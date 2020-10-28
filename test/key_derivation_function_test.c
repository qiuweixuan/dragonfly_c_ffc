#include "../include/std_common.h"
#include "../include/dragonfly_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"
#include "../include/ffc_group.h"


int main()
{
    // compute_hashed_password
    uint8_t client_random[] = {0x02,0x00,0x00,0x00,0x01,0x00}; 
    uint8_t server_random[] = {0x02,0x00,0x00,0x00,0x00,0x00}; 
    Client_Server_Random randoms = {
        .client_random = client_random,
        .server_random = server_random,
        .random_len = sizeof(client_random)
    };
    uint8_t counter = 1;
    uint8_t digest[HMAC_SHA256_DIGEST_SIZE];
    CString cstring_digest = {
        .str = digest,
        .len = sizeof(digest)
    };


    uint8_t password_str[] = "abcdefgh";
    Password pwd = {
        .str = password_str,
        .len = strlen(password_str)
    };

    compute_hashed_password(&cstring_digest,&randoms,&pwd,counter);

    printf("digest:\n");
    print_hex_str(digest,sizeof(digest));


    // key_derivation

    // 上一步计算出的摘要值
    CString base = {
        .str = digest,
        .len = HMAC_SHA256_DIGEST_SIZE
    };
    // 用于伪随机数派生密钥的字符串种子
    CString str_for_seed = {
        .str = "SAE Hunting and Pecking"
    };
    str_for_seed.len = strlen(str_for_seed.str);
    
    // 群的素数充当数据
    const FFC_Group* group15 = ffc_group_factory(DH_GROUP15);
    CString data = {
        .str = (uint8_t*) group15->prime,
        .len = group15->group_len
    };

    // 位数
    uint16_t n = group15->group_len * 8;

    // 返回结果
    uint8_t derived_ptr[group15->group_len];
    CString derived_key = {
        .str = derived_ptr,
        .len = group15->group_len
    };
    // void sha256_prf_bits(CString* result,const CString* key, CString* label,  CString* data, const uint16_t buf_len_bits)
    sha256_prf_bits(&derived_key,&base,&str_for_seed,&data,n);

    printf("derived key:\n");
    cstring_print_hex_str(&derived_key);

    // 输出派生的gmp_key
    mpz_t gmp_key;
    mpz_init(gmp_key);
    cstring_set_mpz(gmp_key,&derived_key);
    gmp_printf("gmp_key  = %Zx\n", gmp_key);
    mpz_clear(gmp_key);


    return 0;
}
