#include "../include/dragonfly_struct.h"
#include <time.h>

// 获取gmp随机状态的单例模式
gmp_randstate_t* get_randstate()
{
    static gmp_randstate_t randstate;
    static int is_init = 0;

    if(!is_init)
    {
        // printf("is_init\n");
        is_init = 1;
        clock_t time = clock();
        gmp_randinit_default(randstate);
        gmp_randseed_ui(randstate, time);
    }
    
    return &randstate;
}

void init_privmask(Priv_Mask* priv_mask)
{
    mpz_init(priv_mask->priv);
    mpz_init(priv_mask->mask);
}

void set_privmask(Priv_Mask* priv_mask,const GMP_FFC_Group *ffc_group)
{
    
    //# each party chooses two random numbers, private and mask
    // 获取全局单例随机数状态
    gmp_randstate_t* randstate_ptr = get_randstate();

    mpz_urandomm(priv_mask->priv, *randstate_ptr, ffc_group->gmp_order);
    if (mpz_cmp_ui(priv_mask->priv, 2) < 0)
    {
        mpz_set_ui(priv_mask->priv, 2);
    }
    mpz_urandomm(priv_mask->mask, *randstate_ptr, ffc_group->gmp_order);
    if (mpz_cmp_ui(priv_mask->mask, 2) < 0)
    {
        mpz_set_ui(priv_mask->mask, 2);
    }
}

void clear_privmask(Priv_Mask* priv_mask)
{
    mpz_clear(priv_mask->priv);
    mpz_clear(priv_mask->mask);
}


void init_commit_element(Commit_Element* commit_element)
{
    mpz_init(commit_element->scalar);
    mpz_init(commit_element->element);
}

void clear_commit_element(Commit_Element* commit_element)
{
    mpz_clear(commit_element->scalar);
    mpz_clear(commit_element->element);
}


