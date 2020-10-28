#ifndef DRAGONFLY_OP_H
#define DRAGONFLY_OP_H

#include "std_common.h"
#include "cstring.h"
#include "ffc_group.h"
#include "dragonfly_struct.h"


// init_pwe过程 / 计算哈希值子过程
void compute_hashed_password(CString* digest,const Client_Server_Random* randoms,const Password* password ,const uint8_t counter);

// init_pwe整个过程 
void init_pwe(mpz_t password_element, const Client_Server_Random *randoms, const Password *password, const GMP_FFC_Group *ffc_group, const uint8_t k);

// commit_exchange整个过程
void commit_exchange(Commit_Element* commit_element ,Priv_Mask* priv_mask,const GMP_FFC_Group *ffc_group,const mpz_t password_element);

// compute_shared_secret 整个过程
void compute_shared_secret(mpz_t shared_secret,CString* token,
    mpz_t password_element,
    const Priv_Mask* priv_mask,const GMP_FFC_Group* ffc_group,
    const Commit_Element* commit_element,const Commit_Element* peer_commit_element);

#endif