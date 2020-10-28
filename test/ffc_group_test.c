#include "../include/std_common.h"
#include "../include/ffc_group.h"


int main()
{
    // ffc_group_factory test
    const FFC_Group* group5 = ffc_group_factory(DH_GROUP5);
    printf("group5 len :%u \n",group5->group_len);
    const FFC_Group* group15 = ffc_group_factory(DH_GROUP15);
    printf("group15 len :%u \n",group15->group_len);

    // gmp_ffc_group_factory_init test
    GMP_FFC_Group* gmp_group5 = (GMP_FFC_Group*)malloc(sizeof(GMP_FFC_Group));
    gmp_ffc_group_factory_init(gmp_group5,DH_GROUP5);
    gmp_printf("gmp_group5 prime = %Zx\n", gmp_group5->gmp_prime);
    gmp_printf("gmp_group5 order = %Zx\n", gmp_group5->gmp_order);
    gmp_ffc_group_clear(gmp_group5);
    free(gmp_group5);

    GMP_FFC_Group* gmp_group15 =  (GMP_FFC_Group*)malloc(sizeof(GMP_FFC_Group));
    gmp_ffc_group_factory_init(gmp_group15,DH_GROUP15);
    gmp_printf("gmp_group15 prime = %Zx\n", gmp_group15->gmp_prime);
    gmp_printf("gmp_group15 order = %Zx\n", gmp_group15->gmp_order);
    gmp_ffc_group_clear(gmp_group15);
    free(gmp_group15);

    return 0;
}