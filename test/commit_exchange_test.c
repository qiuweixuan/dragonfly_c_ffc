

#include "../include/std_common.h"
#include "../include/dragonfly_op.h"
#include "../include/cstring.h"
#include "../include/ssl.h"

#include <assert.h>

int main()
{
    gmp_randstate_t* ptr1 = get_randstate();
    gmp_randstate_t* ptr2 = get_randstate();

    // 使用断言进行判断
    assert(ptr1 != NULL);
    assert(ptr1 == ptr2);

    // 单元测试输入初始化
    uint8_t str_pass [] = "c40eb7628785c0c8eee789dc564bfa3cf4ad22a0de16324259ec20942b888925ce1b12340262af4205af0a464a754f2586b3a228f5f6abe0fe0a51947640a73346289d45a0fd07eafe0b8cb96dc436ab9a36e9779b675ded301794b10e840f39c8776c168d0537b41de8b94e732825ea055fa782795df215b42e263611f899c98bd5166157113b0ad492a4138de5876ca0c6107d4645cf2f7bfc71c78b69a820ac8f1a96b708818df0f9a73962bec59c0b7630a1824fb507d494b8033a78408f7a1abb7db12723cc798c520d0bd532a9309da3d360c78147496b8015b2979598b50914cf776e21d5ad0fa8b1e7f85e7624b896d1f37fd4d837a9d5e7154bca7cfa079df6d12cfe41a0842ff385954fb612d20b34f3c6ce6fa0c30df3a1ed8709228a8710e37b6579e56d64b4322f79338e8765d05d91f5e97e5c3246afdc2702bcd5ced4c1e546d279739d78be467811b8d2f23259704cc680910656c8280a01d3a2c4c93190056d343ee47dc2bad9ae78c3600a2e3490e2773b03ad80027db4";
    uint8_t sta_priv [] = "369de1458282fb76548b6b88d25eb1b39b85202e58780004c500d9e2d448cee023a41e01e4a3857be297ca9b0aeb710fac965a192ca450f2b1d045919e1540096e144f31b08aa5567585509b5af417cc3aeca5d5d1e7b4d9bd08a23b21e10eda59a30ef9417227f26f19b2314e20edc31e237e4151737f479aa7f2af1374d9919ba95afe819a522f33ec746714a7965792b93ebd3e3b23b3dcf5b6c033fe6e71a73d0727e49b294bb104c6dfb3d0fa6c3033f6283b1e84527ddb3e851cd3e86ee3bdd4198166905d628ac18556944861a190faeac7a4b5a39d0090533db26e24aea4785b1924a8a2b72e774d96506a6769e29a29525463406a8da99e57700409266ed3d3224e69721c02776df21a831f5278b0e8a0589e9c57506224f675483b9c1a10e29842bb3601de3e4b495a67860dfa67ced8e006818cff92609e03444737a63eedf7eff4712e8954085d696de733aeca74b829c0a498ccb17edf4d33f727aecbd5313b1530154a5950a0db811e09ac5301ca4404e12e39d010dcb57a4d";
    uint8_t sta_mask [] = "6aca2ccb867cc9b18827a0ccebf976c52d8eef21b526f8149a5623890f91620201a25e44a62a3bd54a580ab866f1305e8ac080fb0cea31912ecab35ff40c11dacd7601d0247f3a615ebb117f9bafd7e888c8625686392f926fa8c30922f1d399752efe97e97b7de09b0b68dabbe15f53127fe8049160e9ff5c3baaf48de0c6c93f6e36139c7e29d12027e75750003a8da96d3c173399116c94229add40f124f90fdb8f4087713ff748762f0b210632ac9b186218e0dfb7be00bc2038f387ea2395926cca22db6b21501c48242df8cac78ac0c50a6499556867dd86c9225a6b67ef914e6b117b19435371d86010740527be3aa5c258ac707a764d632887fcf096c4c1ee2037edcf038a00e9a8446bb54d93dec995797252fb897065d36302257713128a87dfb056606814e3b8ddc6db19097bffdcc9f9113d47f11ca0e6f72339af8c0b1030ff9117b8629dffa2171ddf6f3004f1eb4beaae6bab20146e7f06b4987abe2cb42585014ab203544e82f47f4421711dd7fa8c43dd3124159cf1e065";
   

    // 初始化mpz
    mpz_t password_element;
    Priv_Mask sta_privmask;

    // 分配内存
    mpz_init(password_element);
    init_privmask(&sta_privmask);

     // 使用string初始化mpz
    mpz_set_str(password_element,str_pass,16);
    mpz_set_str(sta_privmask.priv,sta_priv,16);
    mpz_set_str(sta_privmask.mask,sta_mask,16);
    

    // 有限域元素
    GMP_FFC_Group gmp_group15;
    gmp_ffc_group_factory_init(&gmp_group15,DH_GROUP15);

    // 提交元素
    Commit_Element commit_element;
    init_commit_element(&commit_element);

    // 提交元素计算
    commit_exchange(&commit_element,&sta_privmask,&gmp_group15,password_element);

    // 释放内存
    clear_commit_element(&commit_element);
    gmp_ffc_group_clear(&gmp_group15);
    clear_privmask(&sta_privmask);
    mpz_clear(password_element);

    return 0;
}
