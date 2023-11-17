#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "mem.h"
#include "string.h"
#include "console.h"


///////////////////////////////////////////////////////////////////////////////
// Static Helper Prototypes
///////////////////////////////////////////////////////////////////////////////
static pagetable_t make_kernel_pagetable(void);
static pte_t * walk_pgtable(pagetable_t pagetable, uint64 va, int alloc);
static void kernel_map_pages(pagetable_t, uint64, uint64, uint64, int);
static int  kernel_map_range(pagetable_t, uint64, uint64, uint64, int);
static void free_range(void *, void *);



///////////////////////////////////////////////////////////////////////////////
// Global Definitions
///////////////////////////////////////////////////////////////////////////////
extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct frame {
  struct frame *next;
};

struct frame *frame_table;

/*
 * the kernel's page table.
 */
pagetable_t kernel_pagetable;

extern char etext[];  // kernel.ld sets this to end of kernel code.

extern char trampoline[]; // trampoline.S


///////////////////////////////////////////////////////////////////////////////
// Page Allocation and Virtual Memory API
///////////////////////////////////////////////////////////////////////////////

// Initialize Virtual memory and activate paging.
void
vm_init(void)
{
  free_range(end, (void*)PHYSTOP);
  kernel_pagetable = make_kernel_pagetable();
  w_satp(MAKE_SATP(kernel_pagetable));
  sfence_vma();
}


// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
vm_page_alloc(void)
{
  // This function should return the first frame from the frame_table
  // linked list. It shoudl also advance the frame_table pointer to
  // the next available frame. Be sure to check to see if a frame is
  // available, and return 0 if it is not. For a hint of how the frame
  // table is structured, please see the "free_range" helper fucntion.
  // YOUR CODE HERE

  return 0;
}


// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to vm_page_alloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
vm_page_free(void *pa)
{
  // This function should link this physical page back into the frame
  // table. The deallocated page should be come the first free frame
  // in the table.
  // YOUR CODE HERE
}


// create an empty page table.
// returns 0 if out of memory.
pagetable_t
vm_create_pagetable()
{
  // This function should do the following:
  //   1.) Allocate a page frame to store the table.
  //   2.) Set the contents of the entire page to zero, thus 
  //      marking every PTE as invalid.
  // If a page cannot be allocated, this function should return 0.
  // YOUR CODE HERE

  return 0;
}


// Look up a virtual address, return the physical address,
// or 0 if not mapped.
uint64
vm_lookup(pagetable_t pagetable, uint64 va)
{
  // This function should use walk_pgtable to look up a page table
  // entry for the given virtual address. If the entry is invalid,
  // return 0. If the entry is valid, convert it to a physical 
  // address and return the physical address.
  // HINT: This function is subtely different than the corresponding
  //       function in XV6. Take care when copying!
  // YOUR CODE HERE

  return 0;
}


// Insert a page into the page table which maps the page containing
// the virtual address va onto the page frame at the physical address
// pa. va can be any arbitrary address, but pa must be aligned to
// a physical page. Returns 0 on success, -1 on failure.
int
vm_page_insert(pagetable_t pagetable, uint64 va, uint64 pa, int perm)
{
    // This function should round va down to to a page address. After
    // doing this, use walk_pgtable to find the correct page table
    // entry. If the address is already present, panic with the
    // message 'remap'. If the address is not present, add the pte for
    // the mapping with permissions specified by perm.
    // Return 0 on success, -1 on failure. 
    // HINT: You will have to use walk_pgtable's allocation abilities.
    //       Ask yourself, how can this function fail without
    //       panicking?
    // YOUR CODE HERE

    return 0;
}


// Remove npages of mappings starting from va. va must be
// page-aligned. The mappings must exist.
// Optionally free the physical memory.
void
vm_page_remove(pagetable_t pagetable, uint64 va, uint64 npages, int do_free)
{
  // This function should use walk_pgtable to find the corresponding
  // entry for the virtual address va. If the pte is not found, or if
  // the page is not present, then this function should panic. If
  // do_free is set to 1, this function should deallocate the
  // corresponding physical page frame.
  // YOUR CODE HERE
}


// Map a block of virtual memory which is size bytes long and begins
// at virtual address va. This function allocates page frames as
// needed. Each allocated page recieves permissions specified by perm.
// On success, this function returns 0, on failure it returns -1.
int
vm_map_range(pagetable_t pagetable, uint64 va, uint64 size, int perm)
{
    // Loop through each virtual address pages. Note that va is not
    // necessarily page alligned, and so you must round it down.
    // We will allocate a new physical page frame for each page, and
    // then use vm_page_insert to add the page to the table.
    // YOUR CODE HERE
    return -1;
}


///////////////////////////////////////////////////////////////////////////////
// Static Helper Functions
///////////////////////////////////////////////////////////////////////////////

// Make a direct-map page table for the kernel.
static pagetable_t
make_kernel_pagetable(void)
{
  pagetable_t kpgtbl;

  kpgtbl = (pagetable_t) vm_page_alloc();
  memset(kpgtbl, 0, PGSIZE);

  // uart registers
  kernel_map_pages(kpgtbl, UART0, UART0, PGSIZE, PTE_R | PTE_W);

  // virtio mmio disk interface
  kernel_map_pages(kpgtbl, VIRTIO0, VIRTIO0, PGSIZE, PTE_R | PTE_W);

  // PLIC
  kernel_map_pages(kpgtbl, PLIC, PLIC, 0x400000, PTE_R | PTE_W);

  // map kernel text executable and read-only.
  kernel_map_pages(kpgtbl, KERNBASE, KERNBASE, (uint64)etext-KERNBASE, PTE_R | PTE_X);

  // map kernel data and the physical RAM we'll make use of.
  kernel_map_pages(kpgtbl, PGROUNDUP((uint64)etext), PGROUNDUP((uint64)etext),
  PHYSTOP-PGROUNDUP((uint64)etext), PTE_R | PTE_W);

  // map the trampoline for trap entry/exit to
  // the highest virtual address in the kernel.
  kernel_map_pages(kpgtbl, TRAMPOLINE, (uint64)trampoline, PGSIZE, PTE_R | PTE_X);


  return kpgtbl;
}



// Return the address of the PTE in page table pagetable
// that corresponds to virtual address va.  If alloc!=0,
// create any required page-table pages.
//
// The risc-v Sv39 scheme has three levels of page-table
// pages. A page-table page contains 512 64-bit PTEs.
// A 64-bit virtual address is split into five fields:
//   39..63 -- must be zero.
//   30..38 -- 9 bits of level-2 index.
//   21..29 -- 9 bits of level-1 index.
//   12..20 -- 9 bits of level-0 index.
//    0..11 -- 12 bits of byte offset within the page.
static pte_t * 
walk_pgtable(pagetable_t pagetable, uint64 va, int alloc)
{
  if(va >= MAXVA)
    panic("walk_pgtable");

  for(int level = 2; level > 0; level--) {
    pte_t *pte = &pagetable[PX(level, va)];
    if(*pte & PTE_V) {
      pagetable = (pagetable_t)PTE2PA(*pte);
    } else {
      if(!alloc || (pagetable = (pde_t*)vm_page_alloc()) == 0)
        return 0;
      memset(pagetable, 0, PGSIZE);
      *pte = PA2PTE(pagetable) | PTE_V;
    }
  }
  return &pagetable[PX(0, va)];
}


// add a mapping to the kernel page table.
// only used when booting.
// does not flush TLB or enable paging.
static void
kernel_map_pages(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
{
  if(kernel_map_range(kpgtbl, va, sz, pa, perm) != 0)
    panic("kernel_map_pages");
}


// Create PTEs for virtual addresses starting at va that refer to
// physical addresses starting at pa. va and size might not
// be page-aligned. Returns 0 on success, -1 if walk_pgtable() couldn't
// allocate a needed page-table page.
static int
kernel_map_range(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
{
  // This is very similar to the vm_map_range function. The only
  // difference is that this function is given the first physical page
  // in pa, and it will map continiguous page frames beginning at this
  // address. It will not use vm_page_alloc as the memory used by this
  // function is already set aside by the freerange for the use of the
  // kernel. This function will only be used at boot time.
  // YOUR CODE HERE
  return -1;
}


static void
free_range(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE) {
    vm_page_free(p);
  }
}


///////////////////////////////////////////////////////////////////////////////
// Unit Tests below this line. Do not change any part of the
// following!
///////////////////////////////////////////////////////////////////////////////

// Run unit tests on the virtual memory system
void
vm_test()
{
    void *p0, *p1, *p2;
    struct frame *fl;
    int passed;
    uint64 big_address = 0x90000000;
    int result;
    int perm = PTE_R | PTE_W;

    //allocation tests
    printf("vm_page_alloc test...");
    passed = 1;
    p0 = vm_page_alloc();
    p1 = vm_page_alloc();
    p2 = vm_page_alloc();
    if(!p0 || !p1 || !p2) {
        passed = 0;
    }

    //steal the free pages
    fl = frame_table;
    frame_table = 0;

    // try to allocate memory
    if(vm_page_alloc()) {
        passed = 0;
    }
    print_pass(passed);
    frame_table = fl;


    //deallocation test
    printf("vm_page_free test...");
    passed = 1;
    vm_page_free(p2);
    if(frame_table != p2) passed = 0;
    vm_page_free(p0);
    if(frame_table != p0) passed = 0;
    vm_page_free(p1);
    if(frame_table != p1) passed = 0;
    print_pass(passed);


    //lookup test
    printf("vm_lookup test...");
    passed = 1;
    if(vm_lookup(kernel_pagetable, (uint64)p0) != (uint64) p0) {
        passed = 0;
    }
    if(vm_lookup(kernel_pagetable, big_address) != 0) {
        passed = 0;
    }
    print_pass(passed);


    //insert test
    printf("vm_page_insert test...");
    passed = 1;
    p0 = vm_page_alloc();
    fl = frame_table;
    frame_table = 0;
    result = vm_page_insert(kernel_pagetable, big_address, (uint64)p0, perm);
    if(result != -1) {
        passed = 0;
    }
    frame_table = fl;
    result = vm_page_insert(kernel_pagetable, big_address, (uint64)p0, perm);
    if(result != 0 || vm_lookup(kernel_pagetable, big_address) == 0) {
    printf("fail 2\n");
        passed = 0;
    }
    *((int*)big_address) = 0x2a;
    if(*((int*)big_address) != 0x2a) {
    printf("fail 3\n");
        passed = 0;
    }
    print_pass(passed);


    //remove test
    printf("vm_page_remove test...");
    passed = 1;
    vm_page_remove(kernel_pagetable, big_address, 1, 1); 
    if(frame_table != p0){
        passed = 0;
    }
    if(vm_lookup(kernel_pagetable, big_address) != 0) {
        passed = 0;
    }
    p0 = vm_page_alloc();
    vm_page_insert(kernel_pagetable, big_address, (uint64)p0, perm);
    vm_page_remove(kernel_pagetable, big_address, 1, 0); 
    if(frame_table == p0) {
        passed = 0;
    }
    if(vm_lookup(kernel_pagetable, big_address) != 0) {
        passed = 0;
    }
    print_pass(passed);
}
