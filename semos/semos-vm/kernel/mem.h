void            vm_init(void);
void*           vm_page_alloc(void);
void            vm_page_free(void *);
pagetable_t     vm_create_pagetable(void);
uint64          vm_lookup(pagetable_t, uint64);
int             vm_page_insert(pagetable_t, uint64, uint64, int);
void            vm_page_remove(pagetable_t, uint64, uint64, int);
int             vm_map_range(pagetable_t, uint64, uint64, int);

void            vm_test();

