#include "../include/dragonfly_op.h"
#include "../include/ssl.h"
#include "../include/cstring.h"
#include "../include/ffc_group.h"
#include <time.h>

// 计算密码哈希值
void compute_hashed_password(CString *digest, const Client_Server_Random *randoms, const Password *password, const uint8_t counter)
{
    uint8_t *min_random;
    uint8_t *max_random;
    uint8_t *tmp_ptr = NULL;

    // key = max_random | min_random
    uint32_t key_len = randoms->random_len * 2;
    // 分配内存
    uint8_t *key = malloc(key_len * sizeof(uint8_t));
    tmp_ptr = key;
    // 比较大小
    if (memcmp(randoms->client_random, randoms->server_random, randoms->random_len) < 0)
    {
        max_random = randoms->server_random;
        min_random = randoms->client_random;
    }
    else
    {
        max_random = randoms->client_random;
        min_random = randoms->server_random;
    }
    memcpy(tmp_ptr, max_random, randoms->random_len);
    tmp_ptr += randoms->random_len;
    memcpy(tmp_ptr, min_random, randoms->random_len);

    // message =  password |counter
    uint32_t message_len = password->len + sizeof(counter);
    // 分配内存
    uint8_t *message = malloc(message_len * sizeof(uint8_t));
    tmp_ptr = message;
    // 拷贝数据
    memcpy(tmp_ptr, password->str, password->len);
    tmp_ptr += password->len;
    memcpy(tmp_ptr, &counter, sizeof(counter));

    // printf("key:\n");
    // print_hex_str(key,key_len);

    // printf("message:\n");
    // print_hex_str(message,message_len);

    // 计算哈希值
    hmac_sha256(digest->str, key, key_len, message, message_len);

    // printf("digest:\n");
    // print_hex_str(digest,32);

    // 释放内存
    free(message);
    free(key);
}

/* 
    """
        See algorithm in https://tools.ietf.org/html/rfc7664
        in section 3.2.1`
    """
*/
/**
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
void init_pwe(mpz_t password_element, const Client_Server_Random *randoms, const Password *password, const GMP_FFC_Group *ffc_group, const uint8_t k)
{

    // 群的比特位数
    uint32_t n = ffc_group->base->group_len * 8;
    gmp_printf("n = %d\n", n);

    // 寻找PWE字符串
    uint8_t str_for_kdf[] = "SAE Hunting and Pecking";
    CString cstring_for_kdf = {
        .str = str_for_kdf,
        .len = strlen(str_for_kdf)};

    // 寻找过程中所需的gmp运算中间变量
    mpz_t exp, temp, seed;
    mpz_init(exp), mpz_init(temp), mpz_init(seed);

    // 摘要值
    uint8_t digest[HMAC_SHA256_DIGEST_SIZE];
    CString cstring_base = {
        .str = digest,
        .len = sizeof(digest)};
    // 派生密钥
    uint8_t derived_ptr[ffc_group->base->group_len];
    CString cstring_derived_key = {
        .str = derived_ptr,
        .len = ffc_group->base->group_len};
    // 伪随机函数所需data字段
    CString cstring_data_kdf = {
        .str = (uint8_t *)ffc_group->base->prime,
        .len = ffc_group->base->group_len};

    // 寻找过程
    int found = 0;
    for (uint8_t counter = 1; counter <= k || found == 0; counter++)
    {
        //base = H(max(Alice,Bob) | min(Alice,Bob) , password | counter)
        // compute_hashed_password(CString* digest,const Client_Server_Random* randoms,const Password* password ,const uint8_t counter)
        compute_hashed_password(&cstring_base, randoms, password, counter);
        printf("pwd-seed = ");
        cstring_print_hex_str(&cstring_base);

        // temp = KDF-n(base,str_for_kdf,cstring_data_kdf,n)
        sha256_prf_bits(&cstring_derived_key, &cstring_base, &cstring_for_kdf, &cstring_data_kdf, n);
        cstring_set_mpz(temp, &cstring_derived_key);

        gmp_printf("pwd-value = %Zx\n", temp);

        found = 1;

        /*
            //seed = (temp mod(p - 1)) + 1 mpz_sub_ui(seed, this->prime, 1);
            mpz_sub_ui(seed, this->prime, 1);
            mpz_mod(seed, temp, seed);
            mpz_add_ui(seed, seed, 1);
        */
        // seed = temp mod prime
        mpz_mod(seed, temp, ffc_group->gmp_prime);

        // gmp_printf("seed = %Zx\n", seed);

        //temp = seed ^ ((prime - 1) / order) mod prime
        if (ffc_group->base->safe_prime)
        {
            /*
                * exp = (prime - 1) / 2 for the group used here, so this becomes:
                * password_element (temp) = seed ^ 2 modulo prime
                */
            mpz_set_ui(exp, 2);
        }
        else
        {
            /* Calculate exponent: (prime - 1) / order */
            mpz_sub_ui(exp, ffc_group->gmp_prime, 1);
            mpz_div(exp, exp, ffc_group->gmp_order);
        }
        // gmp_printf("exp = %Zx\n", exp);
        mpz_powm(temp, seed, exp, ffc_group->gmp_prime);
        //gmp_printf("temp = %Zx\n", temp);

        if (mpz_cmp_ui(temp, 1) > 0)
        {
            mpz_set(password_element, temp);
            found = 1;
        }
    }
    gmp_printf("password_element = %Zx\n", password_element);
    mpz_clear(temp), mpz_clear(seed), mpz_clear(exp);
}

/**
 * 功能：获取提交元素commit_element
 * 输入：
 * 1.用于生成提交元素的随机数：Priv_Mask* priv_mask
 * 2.有限域运算元素：const GMP_FFC_Group* ffc_group
 * 3.init_pwe过程生成的密码元素：const mpz_t password_element
 * 输出：
 * 1.本方提交元素：Commit_Element* commit_element
 * 
 */
void commit_exchange(Commit_Element* commit_element ,Priv_Mask* priv_mask,const GMP_FFC_Group *ffc_group,const mpz_t password_element)
{

    //# each party chooses two random numbers, private and mask
    // set_privmask(priv_mask,ffc_group);

    gmp_printf("Private = %Zx \n", priv_mask->priv);
    gmp_printf("Mask = %Zx \n", priv_mask->mask);

    // scalar = (private + mask) modulo q
    mpz_add(commit_element->scalar, priv_mask->priv, priv_mask->mask);
    mpz_mod(commit_element->scalar, commit_element->scalar, ffc_group->gmp_order);

    /* 
        # If the scalar is less than two (2), the private and mask MUST be
        # thrown away and new values generated.  Once a valid scalar and
        # Element are generated, the mask is no longer needed and MUST be
        # irretrievably destroyed. 
    */
    assert(mpz_cmp_ui(commit_element->scalar, 2) >= 0);

    //Element = inverse(scalar-op(mask, PE))
    mpz_powm(commit_element->element, password_element, priv_mask->mask, ffc_group->gmp_prime);
    // gmp_printf("powm = %Zx \n", this->element);
    mpz_invert(commit_element->element, commit_element->element, ffc_group->gmp_prime);

    gmp_printf("Sending scalar and element to the Peer! \n");
    gmp_printf("Scalar = %Zx \n", commit_element->scalar);
    gmp_printf("Element = %Zx \n", commit_element->element);
}

/**
 * 功能：获取本方共享秘密shared_secret和本方确认token
 * 输入：
 * 1.用于生成提交元素的随机数：const Priv_Mask* priv_mask
 * 2.有限域运算元素：const GMP_FFC_Group* ffc_group
 * 3.本方提交元素：const Commit_Element* commit_element
 * 4.对方提交元素：const Commit_Element* peer_commit_element
 * 输出：
 * 1.共享秘密：mpz_t shared_secret
 * 2.确认token：CString* token
 */

void compute_shared_secret(mpz_t shared_secret,CString* token,
    mpz_t password_element,
    const Priv_Mask* priv_mask,const GMP_FFC_Group* ffc_group,
    const Commit_Element* commit_element,const Commit_Element* peer_commit_element)
{

    /*
	 * K = scalar-op(private, (elem-op(scalar-op(peer-commit-scalar, PWE),
	 *                                        PEER-COMMIT-ELEMENT)))
	 * If K is identity element (one), reject.
	 * k = F(K) (= x coordinate)
	 */

    // ss = scalar-op(peer-commit-scalar, PWE)
    //    = PWE ^ peer-commit-scalar
    ffc_scalar_op(shared_secret, peer_commit_element->scalar, password_element,ffc_group);

    // ss = elem-op(ss,PEER-COMMIT-ELEMENT)
    //    = ss * PEER-COMMIT-ELEMENT
    ffc_element_op(shared_secret, shared_secret, peer_commit_element->element,ffc_group);
    

    // ss = scalar-op(private, ss);
    //    = ss ^ private;
    
    gmp_printf("priv = %Zx \n", priv_mask->priv);
    gmp_printf("ss = %Zx \n", shared_secret);
    gmp_printf("prime = %Zx \n", ffc_group->gmp_prime);


    // ffc_scalar_op(shared_secret, priv_mask->priv, shared_secret,ffc_group);

    mpz_powm(shared_secret, shared_secret, priv_mask->priv, ffc_group->gmp_prime);

    gmp_printf("Shared Secret ss = %Zx \n", shared_secret);

    /* keyseed = H(<0>32, k)
	 * KCK || PMK = KDF-512(keyseed, "SAE KCK and PMK",
	 *                      (commit-scalar + peer-commit-scalar) modulo r)
	 * PMKID = L((commit-scalar + peer-commit-scalar) modulo r, 0, 128)
	 */
    // mpz_t scalar_result;
    // mpz_init(scalar_result);

    // mpz_add(scalar_result, this->scalar, peer_scalar);
    // mpz_mod(scalar_result, scalar_result, this->order);
    // gmp_printf("[%s] scalar = %Zx \n", this->name.c_str(), this->scalar);
    // gmp_printf("[%s] peer_scalar = %Zx \n", this->name.c_str(), peer_scalar);
    // gmp_printf("[%s] scalar_result = %Zx \n", this->name.c_str(), scalar_result);

    // std::string scalar_hex = hex_str2str(mpz2str(scalar_result, 16));

    // printf("[%s] scalar_hex = \n", this->name.c_str());
    // for (size_t i = 0; i < scalar_hex.size(); i++)
    // {
    //     printf("%0x", (unsigned char)scalar_hex[i]);
    // }
    // printf("\n");

    // std::string null_key(32, 0);
    // std::string ss_hex = hex_str2str(mpz2str(ss, 16));
    // printf("[%s] ss_hex = \n", this->name.c_str());
    // for (size_t i = 0; i < ss_hex.size(); i++)
    // {
    //     printf("%0x", (unsigned char)ss_hex[i]);
    // }
    // printf("\n");
    // std::string keyseed = hmac_sha256(null_key, ss_hex);

    // std::string str_for_seed = "SAE KCK and PMK";
    // std::string key_buf = sha256_prf_bits(keyseed, str_for_seed, scalar_hex, 64 * 8);
    // this->kck = std::string(key_buf.c_str(), 32);
    // this->pmk = std::string(key_buf.c_str() + 32, 32);
    // this->pmkid = std::string(16, '0');
    // // std::string kck = std::string(key_buf.c_str(), 32);
    // // std::string pmk = std::string(key_buf.c_str() + 32, 32);
    // // std::string pmkid = std::string(16, '0');
    // for (size_t i = 0; i < scalar_hex.size() && i < 16; i++)
    // {
    //     this->pmkid[i] = scalar_hex[i];
    // }

    // printf("[%s] KCK = %s\n", this->name.c_str(), str2hex_str(this->kck).c_str());
    // printf("[%s] Pairwise Master Key(PMK) = %s \n", this->name.c_str(), str2hex_str(this->pmk).c_str());
    // printf("[%s] PMKID = %s \n", this->name.c_str(), str2hex_str(this->pmkid).c_str());

    // std::string token_message;
    // token_message += ss_hex;
    // // printf("[%s] ss_hex = %s \n", this->name.c_str(), str2hex_str(ss_hex).c_str());
    // token_message += hex_str2str(mpz2str(scalar, 16));
    // // printf("[%s] scalar_hex = %s \n", this->name.c_str(), mpz2str(scalar, 16).c_str());
    // token_message += hex_str2str(mpz2str(peer_scalar, 16));
    // // printf("[%s] peer_scalar_hex = %s \n", this->name.c_str(), mpz2str(peer_scalar, 16).c_str());
    // token_message += hex_str2str(mpz2str(element, 16));
    // token_message += hex_str2str(mpz2str(peer_element, 16));
    // //printf("[%s] TokenMessage = %s \n", this->name.c_str(), str2hex_str(token_message).c_str());

    // std::string token = hmac_sha256(kck, token_message);
    // printf("[%s] Token = %s \n", this->name.c_str(), str2hex_str(token).c_str());

    // mpz_set(this->peer_scalar, peer_scalar);
    // mpz_set(this->peer_element, peer_element);
    // this->ss_hex = ss_hex;

    // mpz_clear(scalar_result);
    // mpz_clear(ss);
    // return token;
}