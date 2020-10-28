#include "../include/std_common.h"
#include "../include/dragonfly_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"
#include "../include/ffc_group.h"




void powm(mpz_t r,mpz_t a,mpz_t b,mpz_t m)
{
    mpz_t base,result; 
    mpz_init(base),mpz_init(result);

    mpz_set_ui(result,1);
    mpz_set(base,a);
    
    int len = mpz_sizeinbase(b, 2);
    char *rop = malloc(sizeof(char) * len);
    mpz_get_str(rop, 2, b);

    for (int i = len - 1; i >= 0 ; i--)
    {
        if(rop[i] == '1')
        {
            mpz_mul(result,result,base);
            mpz_mod(result,result,m);
        }
        mpz_mul(base,base,base);
        mpz_mod(base,base,m);
    }
    mpz_set(r,result);
    free(rop);
    mpz_clear(base),mpz_clear(result);
}

void scalar_op_test(void)
{
    char priv[] = "369de1458282fb76548b6b88d25eb1b39b85202e58780004c500d9e2d448cee023a41e01e4a3857be297ca9b0aeb710fac965a192ca450f2b1d045919e1540096e144f31b08aa5567585509b5af417cc3aeca5d5d1e7b4d9bd08a23b21e10eda59a30ef9417227f26f19b2314e20edc31e237e4151737f479aa7f2af1374d9919ba95afe819a522f33ec746714a7965792b93ebd3e3b23b3dcf5b6c033fe6e71a73d0727e49b294bb104c6dfb3d0fa6c3033f6283b1e84527ddb3e851cd3e86ee3bdd4198166905d628ac18556944861a190faeac7a4b5a39d0090533db26e24aea4785b1924a8a2b72e774d96506a6769e29a29525463406a8da99e57700409266ed3d3224e69721c02776df21a831f5278b0e8a0589e9c57506224f675483b9c1a10e29842bb3601de3e4b495a67860dfa67ced8e006818cff92609e03444737a63eedf7eff4712e8954085d696de733aeca74b829c0a498ccb17edf4d33f727aecbd5313b1530154a5950a0db811e09ac5301ca4404e12e39d010dcb57a4d"; 
    char ss[] = "94be06686b897a0078c14e71a88bbeff4439abc643bbb6a0fc7ec55abf8672e32c4d10e8833411e1fac79fe1367aee989fe92ca7855f22c1c2f733caaf33ab19d85e819a6770576aee4e64e991077c00263639af2880f4ae495297659f69b7e187ff3fbef455053f771e8596e16410d53312f686687eb70bb0cc17f1f75a7a295bcff4f703bc4cb5bb568885e70d60d0b1aa886ef576c12e9e4a2063ed29d620ee014cb65361f9540fa86bbc11b613d65bcf506f7b54d008e9aa3424fb362823f7a86a29aab28629105118d64e0a3b09847701d8f3ec21f8d5f4c63a5a862608b24594bae69234a76cd1a33ba293d0bf88edc96fcba92f8a8de58bb6274775034be18133dc8486389832d4e37dd620088943b687d49bff789cd4846312d6e419490c533b42f958cbde5ab7d5e7c132be8aa9c1978bef45ae425af9f2d42874faaaf4a99264543c8f48bc0cd815da3b676bec3c0bc1bd9f9499d5c60febb7c0165bd9a1d46569927b84f0a471c6b7a952cb4995832703dea113b0cb48ca43c2ff";
    char prime[] = "ffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca18217c32905e462e36ce3be39e772c180e86039b2783a2ec07a28fb5c55df06f4c52c9de2bcbf6955817183995497cea956ae515d2261898fa051015728e5a8aaac42dad33170d04507a33a85521abdf1cba64ecfb850458dbef0a8aea71575d060c7db3970f85a6e1e4c7abf5ae8cdb0933d71e8c94e04a25619dcee3d2261ad2ee6bf12ffa06d98a0864d87602733ec86a64521f2b18177b200cbbe117577a615d6c770988c0bad946e208e24fa074e5ab3143db5bfce0fd108e4b82d120a93ad2caffffffffffffffff";

    // char priv[] = "941"; 
    // char ss[] = "747";
    // char prime[] = "178";

    mpz_t mpz_priv,mpz_ss,mpz_prime;
    mpz_init(mpz_priv), mpz_init(mpz_ss),mpz_init(mpz_prime);
    
    mpz_set_str(mpz_priv,priv,16);
    mpz_set_str(mpz_ss,ss,16);
    mpz_set_str(mpz_prime,prime,16);

    // mpz_powm(mpz_ss, mpz_ss, mpz_priv, mpz_prime);
    powm(mpz_ss, mpz_ss, mpz_priv, mpz_prime);
    printf("Shared Secret:\n");
    mpz_print_hex(mpz_ss);

    
    
    mpz_clear(mpz_priv),mpz_clear(mpz_ss),mpz_clear(mpz_prime);

}






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

    // 5.运算结果：密码元素
    mpz_t password_element;
    mpz_init(password_element);
    init_pwe(password_element,&randoms,&pwd,&gmp_group15,k);



    // 1.初始化sta_privmask
    uint8_t sta_priv [] = "369de1458282fb76548b6b88d25eb1b39b85202e58780004c500d9e2d448cee023a41e01e4a3857be297ca9b0aeb710fac965a192ca450f2b1d045919e1540096e144f31b08aa5567585509b5af417cc3aeca5d5d1e7b4d9bd08a23b21e10eda59a30ef9417227f26f19b2314e20edc31e237e4151737f479aa7f2af1374d9919ba95afe819a522f33ec746714a7965792b93ebd3e3b23b3dcf5b6c033fe6e71a73d0727e49b294bb104c6dfb3d0fa6c3033f6283b1e84527ddb3e851cd3e86ee3bdd4198166905d628ac18556944861a190faeac7a4b5a39d0090533db26e24aea4785b1924a8a2b72e774d96506a6769e29a29525463406a8da99e57700409266ed3d3224e69721c02776df21a831f5278b0e8a0589e9c57506224f675483b9c1a10e29842bb3601de3e4b495a67860dfa67ced8e006818cff92609e03444737a63eedf7eff4712e8954085d696de733aeca74b829c0a498ccb17edf4d33f727aecbd5313b1530154a5950a0db811e09ac5301ca4404e12e39d010dcb57a4d";
    uint8_t sta_mask [] = "6aca2ccb867cc9b18827a0ccebf976c52d8eef21b526f8149a5623890f91620201a25e44a62a3bd54a580ab866f1305e8ac080fb0cea31912ecab35ff40c11dacd7601d0247f3a615ebb117f9bafd7e888c8625686392f926fa8c30922f1d399752efe97e97b7de09b0b68dabbe15f53127fe8049160e9ff5c3baaf48de0c6c93f6e36139c7e29d12027e75750003a8da96d3c173399116c94229add40f124f90fdb8f4087713ff748762f0b210632ac9b186218e0dfb7be00bc2038f387ea2395926cca22db6b21501c48242df8cac78ac0c50a6499556867dd86c9225a6b67ef914e6b117b19435371d86010740527be3aa5c258ac707a764d632887fcf096c4c1ee2037edcf038a00e9a8446bb54d93dec995797252fb897065d36302257713128a87dfb056606814e3b8ddc6db19097bffdcc9f9113d47f11ca0e6f72339af8c0b1030ff9117b8629dffa2171ddf6f3004f1eb4beaae6bab20146e7f06b4987abe2cb42585014ab203544e82f47f4421711dd7fa8c43dd3124159cf1e065";
    Priv_Mask sta_privmask;
    init_privmask(&sta_privmask);
    mpz_set_str(sta_privmask.priv,sta_priv,16);
    mpz_set_str(sta_privmask.mask,sta_mask,16);
    

    // 2.初始化提交元素
    Commit_Element sta_commit_element;
    init_commit_element(&sta_commit_element);

    // 3.提交元素计算
    commit_exchange(&sta_commit_element,&sta_privmask,&gmp_group15,password_element);


    // 1.初始化shared_secret
    mpz_t shared_secret;
    mpz_init(shared_secret);

    // 2.初始化token
    uint8_t token[HMAC_SHA256_DIGEST_SIZE];
    CString cstring_token = {
        .str = token,
        .len = sizeof(token)
    };

     // 3.初始化ap_commit_element
    uint8_t ap_scalar [] = "6632ac3abc83ee96ce9cfc1ee809c9bd4ea9dec5e6d891937191428bb63d42fc72528a98e5cbef6ee05ff216aa4d8452fff11700415c6a54050653badd0329eae741db73433b9b00c6f5df568341290714ed5304ed8f7d3691a96a30e51dce469c1c0f8b19edc731e15c2fa96757483c44a737ebdb5da5dd9a57a648f31642ae29b42b6b9190ea30bdc4943d03160b9e0fac0b01f232593304366a5ece91f2fa969765860eaf2789550affa76f79235f6450968dfab9fa5a7c902b3fdd17e5823fedeaf82ad346b587e882aa8834dfe87b21ac033236424fb21d933f183103029f9e6b0e1c073cde411c7468f810db9fd7256b1338e1e8213fe20b9ad196004a4b1c20526dc7d0ddaf6a4d7f9864c711429ba3de788986b1cad48926aed4f3ff9bc02c192d095d435b273743f9db668d88a17a64b7f2c8bb6ced05f7e133d5b8829c3f088dce8ebd684565cac6147e03946495caa87a1a42ef03f9ddfc4aacc8562d4ed1898c9452027dd38f1052e328456b9d14ef4ed84ebd3fd559069c5129";
    uint8_t ap_element [] = "cc341d1af02bb87daa52861d2398b300226644ce4c93a09213ddf0dff67292714c49a5bfc0d5e39c516f45da9938d2f5c399ccc07959b5683493baaa62633d32abafb5bb15f6d79d4aef0f41424597d486242951becd63ce35708abd1538c8bc5319ed5fc99d586f4ace923a33a3c03a6eddeb13565b68e649310580b76687911d6d24ef3d8ffcfcc0bef2d2efc4f00360b873f7598ea4735941d740453acf75a62a61a5f091650baf9d1303319e343d6b5d692362b5fc19ec92035439d08441f9c5b9f0f116899a96d32e03574de3929aea7360bf9ef3640bb3aeea6304332b665bc27fb414fa555bef37fc5566a4a6f742a7f0f8ccb44e56ee6dee7831ac28f73a58ef04fcdb0cd513d14e0b94fc667e24d87ddccd8bb0ad16b89512ac3ee9d9e5904d2837e374b7831418518ea4ba1852342ac60f1737a4fcbdc6971e9753f55b3932e383f5f1dc13d2b25d5172fe129ecfc891445a8e0faeca17c5374028526c7276612aad315a77398cf223fb6492db71f1e89de4d0e2e16856beb366c9";
    Commit_Element ap_commit_element;
    init_commit_element(&ap_commit_element);
    mpz_set_str(ap_commit_element.scalar,ap_scalar,16);
    mpz_set_str(ap_commit_element.element,ap_element,16);


    // 4.计算shared_secret
    compute_shared_secret(shared_secret,&cstring_token,
    password_element,
    &sta_privmask,&gmp_group15,
    &sta_commit_element,&ap_commit_element);

    mpz_print_hex(shared_secret);



    // 释放资源内存
    clear_commit_element(&ap_commit_element);
    mpz_clear(shared_secret);
    clear_commit_element(&sta_commit_element);
    gmp_ffc_group_clear(&gmp_group15);
    clear_privmask(&sta_privmask);
    mpz_clear(password_element);

    scalar_op_test();

    return 0;
}


