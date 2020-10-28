#include "../include/std_common.h"
#include "../include/dragonfly_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"

int main()
{
    
    uint8_t client_random[] = {0x02,0x00,0x00,0x00,0x01,0x00}; 
    uint8_t server_random[] = {0x02,0x00,0x00,0x00,0x00,0x00}; 
    Client_Server_Random randoms = {
        .client_random = client_random,
        .server_random = server_random,
        .random_len = sizeof(client_random)
    };
    uint8_t counter = 1;
    

    uint8_t password_str[] = "abcdefgh";
    Password pwd = {
        .str = password_str,
        .len = strlen(password_str)
    };

    uint8_t digest[HMAC_SHA256_DIGEST_SIZE];
    CString cstring_digest = {
        .str = digest,
        .len = sizeof(digest)
    };


    compute_hashed_password(&cstring_digest,&randoms,&pwd,counter);

    printf("digest:\n");
    print_hex_str(digest,sizeof(digest));
    

    return 0;
}
