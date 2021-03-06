// list memory map
#include <inc/lib.h>

void
umain(int argc, char **argv)
{
    int i, j, k;
    static pde_t *pde = (pde_t *)0xe0000000;
    static pte_t *pte = (pte_t *)0xe0001000;
    
    
    while (1) {
        
        cprintf("\n-------------------- MEMORY MAP: --------------------\n");
        
        for (i=0; i<NENV; i++) {
            if (envs[i].env_status != ENV_FREE && envs[i].env_type != ENV_TYPE_MD) {
                
                cprintf("\n+ Environment %08x:\n", envs[i].env_id);
                
                if (sys_page_map(envs[i].env_id, (void *)uvpd,
                                 0, (void *)pde, PTE_P | PTE_U)) {
                    cprintf("sys_page_map ERROR!!! (1)\n");
                    exit();
                }
                
                for (j=0; j<=PDX(USTACKTOP); j++) {
                    if (pde[j] & PTE_P) {
                        cprintf("    PDX: %04d\n", (unsigned int)j);
                        if (sys_page_map(envs[i].env_id,
                                         (void *)( &uvpt[ PGNUM( PGADDR(j, 0, 0) ) ] ),
                                         0, (void *)pte, PTE_P | PTE_U)) {
                            cprintf("sys_page_map ERROR!!! (2)\n");
                            exit();
                        }
                        for (k=0; k<NPTENTRIES; k++) {
                            if (pte[k] & PTE_P) {
                                cprintf("        PTX: %04d | va: 0x%05x*** --> pa: 0x%05x***\n",
                                        k,
                                        (unsigned int)PGNUM(PGADDR(j, k, 0)),
                                        (unsigned int)PGNUM(PTE_ADDR(pte[k])));
                            }
                        }
                    }
                }
                
            }
        }
        
        cprintf("\n----------------- END OF MEMORY MAP -----------------\n\n");

        sys_yield();
    }

}
