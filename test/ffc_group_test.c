#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include "../include/ffc_group.h"


int main()
{
    // ffc_group_factory test
    const FFC_GROUP* group5 = ffc_group_factory(DH_GROUP5);
    printf("group5 len :%u \n",group5->group_len);
    const FFC_GROUP* group15 = ffc_group_factory(DH_GROUP15);
    printf("group15 len :%u \n",group15->group_len);

    // gmp_ffc_group_factory test
    GMP_FFC_GROUP* gmp_group5 = (GMP_FFC_GROUP*)malloc(sizeof(GMP_FFC_GROUP));
    gmp_ffc_group_factory(gmp_group5,DH_GROUP5);
    gmp_printf("gmp_group5 prime = %Zx\n", gmp_group5->gmp_prime);
    gmp_printf("gmp_group5 order = %Zx\n", gmp_group5->gmp_order);
    free(gmp_group5);

    GMP_FFC_GROUP* gmp_group15 =  (GMP_FFC_GROUP*)malloc(sizeof(GMP_FFC_GROUP));
    gmp_ffc_group_factory(gmp_group15,DH_GROUP15);
    gmp_printf("gmp_group15 prime = %Zx\n", gmp_group15->gmp_prime);
    gmp_printf("gmp_group15 order = %Zx\n", gmp_group15->gmp_order);
    free(gmp_group15);

    return 0;
}