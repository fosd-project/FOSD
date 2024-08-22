#include <intel_paging.h>

void set_page_entry(page_entry_t *page_entry , unsigned int baseaddr_low , unsigned int baseaddr_high , unsigned short flags) {
    page_entry->baseaddress_low_flags = (baseaddr_low & 0xFFFFF000)|(flags & 0xFFF);
    page_entry->baseaddress_high = baseaddr_high;
}

// setup the PML4 entries
void setup_pml4(unsigned long pml4_table_address , int pdpt_entry_count) {
    page_entry_t *pml4_entry = (page_entry_t *)pml4_table_address;
    page_entry_t *pdpt_entry = (page_entry_t *)(pml4_table_address+(PAGE_MAX_ENTRY_COUNT*sizeof(page_entry_t)));
    page_entry_t *pde_entry =  (page_entry_t *)((pml4_table_address+(PAGE_MAX_ENTRY_COUNT*sizeof(page_entry_t)))+(PAGE_MAX_ENTRY_COUNT*sizeof(page_entry_t)));
    /* overall structure of the PML4 entry : 
    * 
    * pml4_table_address
    *        V
    *        +------------------+----------------------+-------------------+
    *        |                  |                      |                   |
    *        |    PML4 Entry    |      PDPT Entry      |     PDE Entry     |
    *        |                  |                      |                   |
    *        +------------------+----------------------+-------------------+
    *         entry count : 1    entry count :          entry_count : 
    *                             pdpt_entry_count       512 x pdpt_entry_count
    *                           x 1                    x 1
    * (2MB PML4)
    * 
    * This page entry are linearly wired to their corresponding physical address. 
    * Linear address of each page exactly corresponds to the physical address. 
    */

    for(unsigned int i = 0; i < PAGE_MAX_ENTRY_COUNT; i++) {
        set_page_entry(&(pml4_entry[i]) , 0x00 , 0x00 , 0x00);
    }
    set_page_entry(pml4_entry , pdpt_entry , 0 , PAGE_PML4ENTRY_FLAGS_P|PAGE_PML4ENTRY_FLAGS_RW);
    
    for(unsigned int i_pdpt = 0; i_pdpt < pdpt_entry_count; i_pdpt++) {
        set_page_entry(&(pdpt_entry[i_pdpt]) , pde_entry , 0 , PAGE_PDPTENTRY_FLAGS_P|PAGE_PDPTENTRY_FLAGS_RW);
        for(unsigned int i_pde = 0; i_pde < PAGE_MAX_ENTRY_COUNT; i_pde++) {
            /* The linear address is consisted of following structure : 
             *
             * 63             47          39 38          30 29        21 20                 0
             * +-------------+--------------+--------------+------------+-------------------+
             * | Sign extend | PML4E offset | PDPTE offset | PDE offset |    page offset    |
             * +-------------+--------------+--------------+------------+-------------------+
             * 
             * One PDE page covers 2MBs of the memory, and one PDPT page covers 2MB*512 = 1GB of the memory
             * Set the physical address that PDE is pointing exactly to the linear address(according to the structure above.)
             */

            unsigned int base_low  = (i_pde << 21)|((i_pdpt & 0b11) << 30);
            unsigned int base_high = (i_pdpt >> 2);
            set_page_entry(&(pde_entry[i_pde]) , base_low , base_high , PAGE_PDENTRY_FLAGS_P|PAGE_PDENTRY_FLAGS_RW|PAGE_PDENTRY_FLAGS_PS);
        }
        pde_entry += PAGE_MAX_ENTRY_COUNT*sizeof(page_entry_t);
    }
}